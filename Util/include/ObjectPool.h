#pragma once
#include <cstdint>

#ifdef PACHE_COMPILER_MSVC
	#include <malloc.h>
	#define aligned_malloc(alignment, size) _aligned_malloc(size, alignment)
	#define aligned_free(ptr) _aligned_free(ptr)
#endif
#if defined(PACHE_COMPILER_GNU) || defined(PACHE_COMPILER_CLANG)
	#include <cstdlib>
	#define aligned_malloc(alignment, size) std::aligned_alloc(alignment, size)
	#define aligned_free(ptr) std::free(ptr)
#endif

namespace Pache
{
	// Template class for an object pool that efficiently allocates and deallocates
	// objects of type T. The object pool uses memory pages (usually 4096 Bytes) 
	// for efficient allocation and minimizes fragmentation.
	// When acquiring objects from the ObjectPool using the acquire function, it is
	// crucial for users to release those objects using the release function when
	// they are no longer needed. ObjectPool does not automatically release objects,
	// and failing to call release may result in memory leaks.
	template <typename T, size_t alignment = 4096, size_t pageSize = 4096>
	class ObjectPool
	{
		static_assert((alignment& (alignment - 1)) == 0, "Alignment must be a power of 2");
		static_assert((pageSize& (pageSize - 1)) == 0, "Page size must be a power of 2");
	public:
		ObjectPool() = default;

		~ObjectPool()
		{
			if (validPool != nullptr)
			{
				aligned_free((void*)(validPool->address));
			}

			if (usedPools == nullptr)
				return;

			Pool* pool = usedPools;
			do
			{
				void* address = (void*)pool->address;
				pool = pool->nextPool;
				aligned_free(address);
			} while (pool != usedPools);
		}

		ObjectPool(const ObjectPool& other) = delete;
		ObjectPool& operator=(const ObjectPool& other) = delete;

		ObjectPool(ObjectPool&& other) noexcept
			: freeBlocks(other.freeBlocks), validPool(other.validPool), usedPools(other.usedPools)
		{
			other.freeBlocks = nullptr;
			other.validPool = nullptr;
			other.usedPools = nullptr;
		}

		ObjectPool& operator=(ObjectPool&& other) noexcept
		{
			if (this != &other) {
				// Move resources from 'other' to 'this'
				freeBlocks = other.freeBlocks;
				validPool = other.validPool;
				usedPools = other.usedPools;

				// Reset 'other'
				other.freeBlocks = nullptr;
				other.validPool = nullptr;
				other.usedPools = nullptr;
			}
			return *this;
		}

		// Look for available blocks in the free blocks list.
		template <typename... Args>
		T* acquire(Args&&... args)
		{
			// Look for blocks in free blocks
			if (freeBlocks != nullptr)
			{
				// Get a block from the free list and update the pool header.
				Block* block = freeBlocks;
				Pool* pool = getPoolHeader(block);
				++pool->count;

				// Remove the block from the free list.
				freeBlocks = freeBlocks->next;

				// Construct the object in the block and return its address.
				return new(block) T(std::forward<Args>(args)...);
			}

			// Look for unused blocks in the valid pool.
			if (validPool != nullptr)
			{
				// Calculate the address of the next available block in the valid pool.
				T* block = (T*)(validPool->address + sizeof(Pool) + sizeof(Block) * validPool->nextOffset);
				++(validPool->count);

				// If the pool is about to be full, move it to the list of used pools.
				if (++(validPool->nextOffset) >= maxOffset)
				{
					// Move the pool to the used pools list.
					if (usedPools != nullptr)
					{
						validPool->nextPool = usedPools;
						validPool->prevPool = usedPools->prevPool;
						usedPools->prevPool->nextPool = validPool;
						usedPools->prevPool = validPool;
					}
					else
					{
						usedPools = validPool;
					}

					usedPools = validPool;

					// Remove from valid pool.
					validPool = nullptr;
				}

				// Construct the object in the block and return its address.
				return new(block) T(std::forward<Args>(args)...);
			}

			// Look for blocks in a new pool.
			// Allocate a new pool and calculate the address of the first block.
			validPool = new(aligned_malloc(alignment, pageSize)) Pool;
			return new((T*)(validPool->address + sizeof(Pool))) T(std::forward<Args>(args)...);
		}

		// Releases an object back to the object pool.
		void release(T* ptr)
		{
			// Destruct object
			ptr->~T();

			// Updating and releasing the pool.
			Pool* pool = getPoolHeader((Block*)ptr);
			--pool->count;

			// If the pool becomes empty, it is removed from the list
			// of used pools and memory is freed.
			if (pool->count == 0)
			{

				if (pool == validPool)
				{
					// Remove from valid pool.
					validPool = nullptr;
				}
				else
				{
					// Remove from the used pools list.
					pool->prevPool->nextPool = pool->nextPool;
					pool->nextPool->prevPool = pool->prevPool;
				}

				// Release memory.
				aligned_free((void*)pool->address);
				return;
			}

			// Put the block into the free blocks list.
			Block* block = (Block*)ptr;
			block->next = freeBlocks;
			freeBlocks = block;
		}
	private:
		// Internal structure to represent a block of memory in the object pool.
		// Each block contains single object of type T.
		struct Block
		{
			union
			{
				T block;							// The object in the block

				// When a block is free, its content is not valid for object construction, and this space is reused
				// to store a pointer to the next free block in the pool.
				Block* next;						// The pointer to the next free block
			};
		};

		// Internal structure to represent a pool of memory pages.
		// Each pool manages a contiguous block of memory and tracks the allocated objects.
		// The pool structure is placed at the beginning of the allocated memory block, and
		// its memory is aligned based on the template parameters (alignment).
		struct Pool
		{
			// Initializes the pool with the start address sets the nextPool and prevPool
			// pointers to the current pool.
			// When a new Pool is constructed, its first block will be allocated immediately,
			// therefore, the count and nextOffset is initilized as 1 (NOT 0).
			Pool()
				: address((uintptr_t)this), count(1), nextPool(this), prevPool(this), nextOffset(1)
			{
			}

			uintptr_t const address;				// The start address of pool
			uint16_t count;							// The number of acquired objects in the pool
			Pool* nextPool;							// The next pool in the list
			Pool* prevPool;							// The previous pool in the list
			uint16_t nextOffset;					// The offset of the next unused object in the pool
		};

		// List of free blocks available for allocation.
		// These blocks were previously acquired and later released.
		Block* freeBlocks = nullptr;

		// The current pool with available free objects.
		// Objects are allocated from this pool when no free blocks are available.
		Pool* validPool = nullptr;

		// List of pools with acquired objects.
		// These pools have been used to allocate objects.
		Pool* usedPools = nullptr;
		static constexpr uint16_t maxOffset = (pageSize - sizeof(Pool)) / sizeof(Block);
	private:
		Pool* getPoolHeader(Block* block)
		{
			return (Pool*)((uintptr_t)block & ~(pageSize - 1));
		}
	};
}