#include "Identifier.h"
#include "xxhash.h"
#include <cstdlib>

namespace Pache
{
	Identifier::EntryAllocator Identifier::EntryAllocator::instance;

	Identifier::Hash::Hash(const char* str, uint16_t size)
		: hash(XXH3_64bits(str, size))
	{
	}

	// Acquires a new MUTEABLE slot for a given string and enlarges the pool if needed.
	Identifier::Slot& Identifier::Pool::acquireSlot(const char* str, uint16_t size, uint32_t offset, uint32_t tag)
	{
		std::unique_lock<std::shared_mutex> lock(rwMutex);

		// Enlarge the slots array when it is nearly full
		if (count > capacity * MAX_ALLOCATION_RATE)
			enlarge();

		// Due to the Pool's adoption of a doubling strategy for capacity expansion,
		// its capacity is guaranteed to be a power of two. So, bitwise operations
		// are used instead of modulo operations.
		uint32_t CAPACITY_MASK = capacity - 1;
		offset = offset & CAPACITY_MASK;						// offset = offset % capacity

		// Utilizing linear probing to locate the first unused slot or a slot
		// that already contains the corresponding content.
		while (true)
		{
			Slot& slot = slots[offset];

			if (!slot.used())
			{
				// Update the count of used slots
				count++;
				return slot;
			}

			else if (slot.contain(str, size, tag))
				return slot;

			offset = (offset + 1) & CAPACITY_MASK;				// offset = (offset + 1) % capacity
		}
	}

	// Enlarges the pool by creating a new larger pool and transferring existing slots.
	void Identifier::Pool::enlarge()
	{
		// Due to the Pool's adoption of a doubling strategy for capacity expansion,
		// its capacity is guaranteed to be a power of two. So, bitwise operations
		// are used instead of modulo operations.
		uint32_t newCapacity = capacity * 2;
		Slot* newSlots = (Slot*)std::calloc(newCapacity, sizeof(Slot));

		uint32_t CAPACITY_MASK = newCapacity - 1;

		// Remapping already used slots to new slots.
		for (uint32_t i = 0; i < capacity; i++)
		{
			Slot& slot = slots[i];
			if (slot.used())
			{
				const Entry* entry = EntryAllocator::getEntry(slot.getHandle());
				Hash hash(entry->data, entry->size);

				uint32_t offset = hash.getOffset() & CAPACITY_MASK;

				// Utilizing linear probing to locate the first unused slot.
				while (newSlots[offset].used())
				{
					offset = (offset + 1) & CAPACITY_MASK;			// offset = (offset + 1) % capacity
				}
				newSlots[offset] = slot;
			}
		}

		// Release the original memory and update capacity data.
		std::free(slots);
		capacity = newCapacity;
		slots = newSlots;
	}

	Identifier::EntryAllocator::EntryAllocator()
		: capacity(16), blockIndex(0), blockOffset(0)
	{
		// Allocate the first memory block.
		blocks = (uint8_t**)std::malloc(capacity * sizeof(uint8_t*));
		blocks[0] = (uint8_t*)std::calloc(1, BLOCK_SIZE);
	}

	Identifier::EntryAllocator::~EntryAllocator()
	{
		// Release all allocated memory blocks.
		for (uint32_t i = 0; i <= blockIndex; i++)
			std::free(blocks[i]);

		std::free(blocks);
	}

	// Acquires a block of memory with a size of 'size'.
	Identifier::EntryHandle Identifier::EntryAllocator::acquireMemory(uint16_t size)
	{
		// Convert the size aligned to two bytes
		size = (size + 2) & ENTRY_ALIGNMENT_MASK;

		// Allocate a block of memory for the new string and use a mutex to ensure mutual exclusion during access.
		std::lock_guard<std::mutex> lock(mutex);

		// If the memory block is not large enough to store this string, allocate a new memory block.
		if (blockOffset + size + sizeof(Entry) >= BLOCK_SIZE)
		{
			// Allocate a new memory block.
			if (capacity <= blockIndex + 1)
			{
				capacity = capacity * 2;
				blocks = (uint8_t**)(std::realloc(blocks, capacity * sizeof(uint8_t*)));
			}

			blocks[++blockIndex] = (uint8_t*)std::calloc(1, BLOCK_SIZE);
			blockOffset = 0;
		}

		EntryHandle handle(blockIndex, blockOffset);
		blockOffset += size + sizeof(Entry);

		// Create an EntryHandle pointing to the memory block.
		return handle;
	}

	// Acquires a new Entry for a given string and information.
	Identifier::EntryHandle Identifier::EntryAllocator::acquireEntryImpl(const char* str, uint16_t size)
	{
		// Calculate the hash value and retrieve the corresponding Pool and Slot.
		Hash hash(str, size);
		Pool& pool = pools[hash.getIndex()];
		Slot& slot = pool.acquireSlot(str, size, hash.getOffset(), hash.getTag());

		// If the slot is already in use, it indicates that the string is already stored in the memory block.
		// Return the EntryHandle pointing to that memory block directly.
		if (slot.used())
			return slot.getHandle();

		// If the slot is unused, it indicates a completely new string that needs to be stored in the memory block.
		// Acquire memory for the new string.
		EntryHandle handle = acquireMemory(size);

		// Copy the string to the memory block.
		Entry* entry = getEntryImpl(handle);
		entry->size = size;
		std::memcpy(entry->data, str, size);

		// Set Slot information for quick retrieval during subsequent creations.
		slot.set(hash.getTag(), handle);

		return handle;
	}

	Identifier::EntryHandle Identifier::EntryAllocator::acquireEntryImpl(const char* str, uint16_t size, Hash hash)
	{
		// Retrieve the corresponding Pool and Slot.
		Pool& pool = pools[hash.getIndex()];
		Slot& slot = pool.acquireSlot(str, size, hash.getOffset(), hash.getTag());

		// If the slot is already in use, it indicates that the string is already stored in the memory block.
		// Return the EntryHandle pointing to that memory block directly.
		if (slot.used())
			return slot.getHandle();

		// If the slot is unused, it indicates a completely new string that needs to be stored in the memory block.
		// Acquire memory for the new string.
		EntryHandle handle = acquireMemory(size);

		// Copy the string to the memory block.
		Entry* entry = getEntryImpl(handle);
		entry->size = size;
		std::memcpy(entry->data, str, size);
	
		// Set Slot information for quick retrieval during subsequent creations.
		slot.set(hash.getTag(), handle);

		return handle;
	}
}
