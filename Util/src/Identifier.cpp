#include "Identifier.h"
#include "farmhash.h"

namespace Pache
{
	// Global unique EntryAllocator.
	Identifier::EntryAllocator* Identifier::EntryAllocator::instance = new Identifier::EntryAllocator;

	// Constructors for the Identifier
	Identifier::Identifier()
		: id(0)
	{
	}

	Identifier::Identifier(const char* str, uint16_t size)
	{
		id = EntryAllocator::acquireEntry(str, size);
	}

	Identifier::Identifier(const char* str)
		: Identifier(str, static_cast<uint16_t>(std::strlen(str)))
	{
	}

	Identifier::Identifier(const std::string& str)
		: Identifier(str.c_str(), static_cast<uint16_t>(str.size()))
	{
	}

	Identifier::Identifier(const Identifier& other)
		: id(other.id)
	{
	}

	Identifier::Identifier(Identifier && other) noexcept
		: id(std::exchange(other.id, 0))
	{
	}

	Identifier& Identifier::operator=(const Identifier& other)
	{
		if (this != &other)
		{
			id = other.id;
		}
		return *this;
	}

	Identifier& Identifier::operator=(Identifier&& other) noexcept
	{
		if (this != &other)
		{
			id = std::exchange(other.id, 0);
		}
		return *this;
	}

	// Returns a pointer to the C-style string representation of the Identifier.
	const char* Identifier::data() const
	{
		return EntryAllocator::getEntry(*(EntryHandle*)&id);
	}

	// Returns a pointer to the C-style string representation of the Identifier.
	const char* Identifier::c_str() const 
	{
		return EntryAllocator::getEntry(*(EntryHandle*)&id);
	}

	// Constructor for the Hash
	Identifier::Hash::Hash(const char* str, uint16_t size)
		: hash(util::Hash64(str, size))
	{
	}

	// Getters for the Hash 
	uint32_t Identifier::Hash::getIndex() const { return (address & HASH_INDEX_MASK) >> HASH_INDEX_OFFSET; }
	uint32_t Identifier::Hash::getOffset() const { return address & HASH_OFFSET_MASK; }
	uint32_t Identifier::Hash::getTag() const { return tag; }

	// Constructor for the EntryHandle
	Identifier::EntryHandle::EntryHandle()
		: handle(0)
	{
	}

	Identifier::EntryHandle::EntryHandle(uint32_t index, uint32_t offset)
		: handle((index << HANDLE_INDEX_OFFSET) | offset | USED_MASK)
	{
	}
	
	// Getters for the EntryHandle
	uint32_t Identifier::EntryHandle::getIndex() const { return (handle & HANDLE_INDEX_MASK) >> HANDLE_INDEX_OFFSET; }
	uint32_t Identifier::EntryHandle::getOffset() const { return handle & HANDLE_OFFSET_MASK; }

	// Checks if the handle is marked as used.
	bool Identifier::EntryHandle::used() const { return handle & USED_MASK; }

	Identifier::EntryHandle::operator uint32_t() const { return handle; }

	// Returns the handle associated with the slot.
	Identifier::EntryHandle& Identifier::Slot::getHandle() { return handle; }

	// Sets the tag, size, and handle for the slot.
	void Identifier::Slot::set(uint32_t tag, uint16_t size, const EntryHandle& handle)
	{
		this->tag = tag;
		this->size = size;
		this->handle = handle;
	}

	uint16_t Identifier::Slot::getSize() const { return size; }

	// Checks if the slot is marked as used.
	bool Identifier::Slot::used() const { return handle.used(); }

	// Checks if the slot contains a specific string based on tag, size, and content.
	bool Identifier::Slot::contain(const char* str, uint16_t size, uint32_t tag) const
	{
		return this->tag == tag && this->size == size && std::strncmp(str, EntryAllocator::getEntry(handle), size) == 0;
	}

	// Constructor for the Pool
	Identifier::Pool::Pool()
		: capacity(INITIAL_SLOTS), count(0), slots((Slot*)std::calloc(INITIAL_SLOTS, sizeof(Slot)))
	{
	}

	// Destructor for the Pool.
	Identifier::Pool::~Pool()
	{
		std::free(slots);
	}

	// Returns a reference to the slot at the specified index.
	const Identifier::Slot& Identifier::Pool::operator[](uint32_t index)
	{
		std::shared_lock<std::shared_mutex> lock(rwMutex);
		return slots[index];
	}

	// Acquires a new MUTEABLE slot for a given string and enlarges the pool if needed.
	Identifier::Slot& Identifier::Pool::acquireSlot(const char* str, uint16_t size, uint32_t offset, uint32_t tag)
	{
		std::unique_lock<std::shared_mutex> lock(rwMutex);
		// Enlarge the slots array when it is nearly full
		if (count > capacity * MAX_ALLOCATION_RATE)
			enlarge();

		// Update the count of used slots
		count++;

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
			if (!slot.used() || slot.contain(str, size, tag))
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

		uint32_t CAPACITY_MASK = capacity - 1;

		// Remapping already used slots to new slots.
		for (uint32_t i = 0; i < capacity; i++)
		{
			Slot& slot = slots[i];
			if (slot.used())
			{
				Entry* entry = EntryAllocator::getEntry(slot.getHandle());
				Hash hash(entry, slot.getSize());

				uint32_t offset = hash.getOffset() & CAPACITY_MASK;

				// Utilizing linear probing to locate the first unused slot.
				while (true)
				{
					if (!slots[offset].used())
					{
						newSlots[offset] = slot;
						break;
					}
					offset = (offset + 1) & CAPACITY_MASK;			// offset = (offset + 1) % capacity
				}
			}
		}
		
		// Release the original memory and update capacity data.
		std::free(slots);
		capacity = newCapacity;
		slots = newSlots;
	}

	Identifier::EntryAllocator::EntryAllocator()
		: blockIndex(0), blockOffset(0)
	{
		// Allocate the first memory block.
		blocks.emplace_back((uint8_t*)std::malloc(BLOCK_SIZE));
	}

	Identifier::EntryAllocator::~EntryAllocator()
	{
		// Release all allocated memory blocks.
		for (uint8_t* block : blocks)
			std::free(block);
	}

	// Enlarges the blocks by creating a new larger block.
	void Identifier::EntryAllocator::enlarge()
	{
		// Allocate a new memory block.
		blocks.emplace_back((uint8_t*)std::malloc(BLOCK_SIZE));
		// Update blocks.
		blockIndex++;
		blockOffset = 0;
	}

	// Gets the Entry associated with a given EntryHandle.
	Identifier::Entry* Identifier::EntryAllocator::getEntryImpl(Identifier::EntryHandle handle) const
	{
		return (Entry*)(blocks[handle.getIndex()] + handle.getOffset());
	}

	// Acquires a new Entry for a given string and information.
	Identifier::EntryHandle Identifier::EntryAllocator::acquireEntryImpl(const char* str, uint16_t size)
	{
		// Calculate the hash value and retrieve the corresponding Pool and Slot.
		Hash hash(str, size);
		Pool& pool = pools[hash.getIndex()];
		Slot& slot = pool.acquireSlot(str, size, hash.getOffset(), hash.getTag());

		// If the slot is unused, it indicates a completely new string that needs to be stored in the memory block.
		if (!slot.used())
		{
			std::lock_guard<std::mutex> lock(mutex);
			// If the memory block is not large enough to store this string, allocate a new memory block.
			if (blockOffset + size >= BLOCK_SIZE - 1)
				enlarge();

			// Create an EntryHandle pointing to the memory block.
			EntryHandle handle(blockIndex, blockOffset);

			// Copy the string to the memory block.
			Entry* entry = getEntryImpl(handle);
			std::memcpy(entry, str, size + 1);

			// Set Slot information for quick retrieval during subsequent creations.
			slot.set(hash.getTag(), size, handle);

			// Update blocks.
			blockOffset += size + 1;

			return handle;
		}

		// If the slot is already in use, it indicates that the string is already stored in the memory block.
		// Return the EntryHandle pointing to that memory block directly.
		return slot.getHandle();
	}
}