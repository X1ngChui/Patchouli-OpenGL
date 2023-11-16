#pragma once
#include <cstdint>
#include <vector>

#ifdef PACHE_COMPILER_MSVC
	#include <malloc.h>
	#define aligned_malloc(alignment, size) _aligned_malloc(size, alignment)
	#define aligned_free(ptr) _aligned_free(ptr)
#endif
#if defined(PACHE_COMPILER_GNU) || defined(PACHE_COMPILER_CLANG)
	#define aligned_malloc(alignment, size) std::aligned_alloc(alignment, size)
	#define aligned_free(ptr) std::free(ptr)
#endif

namespace Pache
{
	template <typename T, size_t alignment = 4096, size_t pageSize = 4096>
	class ObjectPool
	{
	private:
		struct Block
		{
			union
			{
				T block;
				Block* next;
			};
		};

		struct Pool
		{
			Pool()
				: address((uintptr_t)this), count(1), nextPool(this), prevPool(this), nextOffset(1)
			{
			}

			uintptr_t const address;				// The start of Pool
			uint16_t count;							// The number of acquired objects
			Pool* nextPool;							// The next pool in the list
			Pool* prevPool;							// The prev pool in the list
			uint16_t nextOffset;					// The next object never been used
		};

		Block* freeBlocks = nullptr;
		Pool* validPool = nullptr;
		Pool* usedPools = nullptr;
		static constexpr uint16_t maxOffset = (pageSize - sizeof(Pool)) / sizeof(Block);
	public:
		template <typename... Args>
		T* acquire(Args&&... args)
		{
			// Look for blocks in free blocks
			if (freeBlocks != nullptr)
			{
				// Get block address and update pool header
				Block* block = freeBlocks;
				Pool* pool = getPoolHeader(block);
				++pool->count;

				freeBlocks = freeBlocks->next;
				return new(block) T(std::forward<Args>(args)...);
			}

			// Look for unused blocks in valid pool
			if (validPool != nullptr)
			{
				// Get block address and update pool header
				T* block = (T*)(validPool->address + sizeof(Pool) + sizeof(Block) * validPool->nextOffset);
				++(validPool->count);

				// If the pool will be full, put it into used pools list
				if (++(validPool->nextOffset) >= maxOffset)
				{
					// Put into used pool list
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

					// Remove from valid pool
					validPool = nullptr;
				}
				return new(block) T(std::forward<Args>(args)...);
			}

			// Look for blocks in new pool
			// Get block address and update pool header
			validPool = new(aligned_malloc(alignment, pageSize)) Pool;
			return new((T*)(validPool->address + sizeof(Pool))) T(std::forward<Args>(args)...);
		}

		void release(T* ptr)
		{
			// Destruct object
			std::destroy_at(ptr);

			// Updating and release pool
			Pool* pool = getPoolHeader((Block*)ptr);
			--pool->count;
			if (pool->count == 0)
			{

				if (pool == validPool)
				{
					// Remove from valid pool
					validPool = nullptr;
				}
				else
				{
					// Remove from the used pools list
					pool->prevPool->nextPool = pool->nextPool;
					pool->nextPool->prevPool = pool->prevPool;
				}
				// Release memory
				aligned_free((void*)pool->address);
				return;
			}

			// Put into free blocks list
			Block* block = (Block*)ptr;
			block->next = freeBlocks;
			freeBlocks = block;
		}

	private:
		Pool* getPoolHeader(Block* block)
		{
			return (Pool*)((uintptr_t)block & ~(pageSize - 1));
		}
	};
}