#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <mutex>
#include <shared_mutex>

namespace Pache
{
	// Identifier provides fast comparison and lookup for short immutable 
	// strings (up to 4096 bytes, including the null terminator). 
	// Identifier only maintains a single constant char array in memory, 
	// making it suitable for identifiers which require frequent comparisons.
	class Identifier
	{
	public:
		// Default constructor for Identifier.
		Identifier();

		// Constructor for Identifier that takes a const char* and its size.
		// This constructor is used to create an Identifier from a given string and size.
		Identifier(const char* str, uint16_t size);

		// Constructor for Identifier that takes a const char*.
		// This constructor calculates the size of the input string using std::strlen.
		Identifier(const char* str);

		// Constructor for Identifier that takes a const std::string&.
		// This constructor is used to create an Identifier from a given std::string.
		Identifier(const std::string& str);

		// Copy constructor for Identifier.
		// This constructor creates a new Identifier by copying the content of another Identifier.
		Identifier(const Identifier& other);

		// Move constructor for Identifier.
		// This constructor creates a new Identifier by transferring the ownership of the content 
		// from another Identifier. The source Identifier is left in a valid but unspecified state.
		Identifier(Identifier&& other) noexcept;

		// Copy assignment operator for Identifier.
		// This operator assigns the content of another Identifier to the current Identifier.
		Identifier& operator=(const Identifier& other);

		// Move assignment operator for Identifier.
		// This operator transfers the ownership of the content from another Identifier 
		// to the current Identifier. The source Identifier is left in a valid but unspecified state.
		Identifier& operator=(Identifier&& other) noexcept;

		// Comparison operator for Identifier.
		// The comparison of Identifiers is based on memory addresses(in other words, the order of their creation),
		// rather than a typical comparison based on content.
		auto operator<=>(const Identifier& other) const { return id <=> other.id; }
		bool operator<(const Identifier& other) const { return id < other.id; }
		bool operator<=(const Identifier& other) const { return id <= other.id; }
		bool operator>(const Identifier& other) const { return id > other.id; }
		bool operator>=(const Identifier& other) const { return id >= other.id; }
		bool operator==(const Identifier& other) const { return id == other.id; }
		bool operator!=(const Identifier& other) const { return id != other.id; }

		// Returns a pointer to the C-style string representation of the Identifier.
		const char* data() const;
		// Returns a pointer to the C-style string representation of the Identifier.
		const char* c_str() const;
	private:
		using Bytes = uint8_t;

		static constexpr uint32_t USED_MASK = 0x80000000;

		static constexpr uint32_t HASH_INDEX_BITS = 8;
		static constexpr uint32_t HASH_INDEX_OFFSET = 32 - HASH_INDEX_BITS;
		static constexpr uint32_t HASH_INDEX_MASK = 0xffffffff & (0xffffffff << HASH_INDEX_OFFSET);
		static constexpr uint32_t HASH_INDEX_SPACE = 1 << HASH_INDEX_BITS;

		static constexpr uint32_t HASH_OFFSET_BITS = 32 - HASH_INDEX_BITS;
		static constexpr uint32_t HASH_OFFSET_MASK = (1 << HASH_OFFSET_BITS) - 1;

		static constexpr uint32_t INITIAL_SLOTS = 256;

		static constexpr uint32_t HANDLE_OFFSET_BITS = 12;
		static constexpr uint32_t HANDLE_OFFSET_MASK = (1 << 12) - 1;
		static constexpr uint32_t BLOCK_SIZE = 1 << HANDLE_OFFSET_BITS;
		static constexpr uint32_t HANDLE_INDEX_BITS = 31 - HANDLE_OFFSET_BITS;
		static constexpr uint32_t HANDLE_INDEX_MASK = 0x7fffffff & (0xffffffff << HASH_INDEX_OFFSET);
		static constexpr uint32_t HANDLE_INDEX_OFFSET = HANDLE_OFFSET_BITS;

		static constexpr float MAX_ALLOCATION_RATE = 0.9f;

		// Hash class is used to calculate the hash value of a string.
		// The 64-bit hash value is divided into three parts: tag, index, and offset.
		// - tag: Used for a quick comparison before comparing characters individually.
		// - index: Determines which Pool the string's information will be stored in.
		// - offset: Determines which Slot in the Pool the string will be stored in.
		class Hash
		{
		public:
			// Constructor for Hash that takes a const char* and its size.
			// Calculates the 64-bit hash value for the given string.
			Hash(const char* str, uint16_t size);

			uint32_t getIndex() const;
			uint32_t getOffset() const;
			uint32_t getTag() const;

		private:
			union
			{
				uint64_t hash;
				struct
				{
					uint32_t address;			// Combined index and offset.
					uint32_t tag;
				};
			};
		};

		// Entry represents the actual storage for the content of strings.
		using Entry = char;

		// EntryHandle class is used to locate the position of a string within the blocks owned by EntryAllocator,
		// which Identifier holds.
		class EntryHandle
		{
		public:
			// Default constructor for EntryHandle.
			EntryHandle();

			// Constructor for EntryHandle that takes index and offset.
			EntryHandle(uint32_t index, uint32_t offset);

			uint32_t getIndex() const;
			uint32_t getOffset() const;

			// Checks if the handle is marked as used.
			bool used() const;

			operator uint32_t() const;
		private:
			uint32_t handle;
		};

		// Slot class stores summarized information related to strings,
		// indicating whether the string is already stored.
		class Slot
		{
		public:
			// Default constructor for Slot.
			Slot() = default;

			// Returns the handle associated with the slot.
			EntryHandle& getHandle();

			// Sets the tag, size, and handle for the slot.
			void set(uint32_t tag, uint16_t size, const EntryHandle& handle);

			// Returns the size of the stored string.
			uint16_t getSize() const;

			// Checks if the slot is marked as used.
			bool used() const;

			// Checks if the slot contains a specific string based on tag, size, and content.
			bool contain(const char* str, uint16_t size, uint32_t tag) const;
		private:
			uint32_t tag;
			EntryHandle handle;
			uint16_t size;
		};

		// Pool class is a cluster of Slots.
		class Pool
		{
		public:
			// Default constructor for Pool.
			Pool();

			~Pool();

			// Returns a CONST reference to the slot at the specified index.
			const Slot& operator[](uint32_t index);

			// Acquires a new MUTEABLE slot for a given string and enlarges the pool if needed.
			Slot& acquireSlot(const char* str, uint16_t size, uint32_t offset, uint32_t tag);
		private:
			// Enlarges the pool by creating a new larger pool and transferring existing slots.
			void enlarge();

			uint32_t capacity;
			uint32_t count;					// Number of used slots in the pool.
			Slot* slots;
			std::shared_mutex rwMutex;
		};

		// EntryAllocator class is responsible for allocating new Entry instances and mapping EntryHandle to Entry.
		// It is implemented as a singleton.
		class EntryAllocator
		{
		public:
			// Default constructor for EntryAllocator.
			EntryAllocator();

			// Destructor for EntryAllocator.
			~EntryAllocator();

			// Gets the Entry associated with a given EntryHandle.
			static Entry* getEntry(EntryHandle handle) { return instance->getEntryImpl(handle); }

			// Acquires a new Entry for a given string and information.
			static EntryHandle acquireEntry(const char* str, uint16_t size) { return instance->acquireEntryImpl(str, size); }
		private:
			// Enlarges the blocks by creating a new larger block.
			void enlarge();

			Entry* getEntryImpl(EntryHandle handle) const;

			EntryHandle acquireEntryImpl(const char* str, uint16_t size);
		private:
			std::vector<Bytes*> blocks;
			uint32_t blockOffset;
			uint32_t blockIndex;

			Pool pools[HASH_INDEX_SPACE];

			std::mutex mutex;

			static EntryAllocator* instance;
		};
	private:
		friend struct std::hash<Pache::Identifier>;
		uint32_t id;
	};
}

namespace std
{
	template <>
	struct hash<Pache::Identifier> {
		size_t operator()(const Pache::Identifier& obj) const
		{
			return obj.id;
		}
	};
}