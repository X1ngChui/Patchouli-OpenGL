#pragma once

#include <string>
#include <string_view>
#include <mutex>
#include <shared_mutex>
#include "fmt/core.h"
#include "ImmHash.h"

#define LITERAL_IDENTIFIER(str) Pache::Identifier::Literal<sizeof(str), ImmHash::hash(str)>(str)

namespace Pache
{
	// Identifier provides fast comparison and lookup for short immutable 
	// strings (up to 4094 bytes, including the null terminator). 
	// Identifier only maintains a single constant char array in memory, 
	// making it suitable for identifiers which require frequent comparisons.
	class Identifier
	{
	public:
		// Literal class represents a string literal along with its compile-time hash value.
		// The hash value is calculated using a compile-time hash function provided by the ImmHash.h header.
		// This class is used to create Identifier instances from string literals.
		template <uint16_t N, uint64_t H>
		class Literal
		{
		public:
			constexpr Literal(const char(&str)[N])
				: str(str)
			{
			}

			constexpr const char* data() const { return str; }
		private:
			const char* str;
		};

		// Default constructor for Identifier.
		Identifier() = default;

		// Constructor for Identifier that takes a const char* and its size.
		// This constructor is used to create an Identifier from a given string and size.
		Identifier(const char* str, uint16_t size);

		// Constructor for Identifier that takes a string literals.
		// This constructor is used to create an Identifier from a string literals.
		template <uint16_t N>
		Identifier(const char(&str)[N])
			: Identifier(str, N - 1)
		{
		}

		// Constructor for Identifier that takes a Literal.
		// This constructor is designed to create an Identifier from a string literal.
		// It can be conveniently used as follows:
		//   Identifier identifier = LITERAL_IDENTIFIER("Hello Patchouli");
		template <uint16_t N, uint64_t H>
		Identifier(Literal<N, H> str)
			: id(EntryAllocator::acquireEntry(str.data(), N - 1, H))
		{
		}
		
		// Constructor for Identifier that takes a const std::string_view&.
		// This constructor is used to create an Identifier from a given std::string_view.
		Identifier(const std::string_view& str);

		// Constructor for Identifier that takes a const std::string.
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
		const char* c_str() const { return EntryAllocator::getEntry(id)->getData(); }

		// Get the length of the identifier.
		size_t size() const { return EntryAllocator::getEntry(id)->getSize(); }
	private:
		using Bytes = uint8_t;

		static constexpr uint32_t USED_MASK = 0x00000001;

		static constexpr uint32_t HASH_INDEX_BITS = 8;
		static constexpr uint32_t HASH_INDEX_OFFSET = 32 - HASH_INDEX_BITS;
		static constexpr uint32_t HASH_INDEX_MASK = 0xffffffff & (0xffffffff << HASH_INDEX_OFFSET);
		static constexpr uint32_t HASH_INDEX_SPACE = 1 << HASH_INDEX_BITS;

		static constexpr uint32_t HASH_OFFSET_BITS = 32 - HASH_INDEX_BITS;
		static constexpr uint32_t HASH_OFFSET_MASK = (1 << HASH_OFFSET_BITS) - 1;

		static constexpr uint32_t INITIAL_SLOTS = 256;
		static constexpr float MAX_ALLOCATION_RATE = 0.8f;

		static constexpr uint32_t HANDLE_OFFSET_BITS = 12;
		static constexpr uint32_t HANDLE_OFFSET_MASK = (1 << HANDLE_OFFSET_BITS) - 1;
		static constexpr uint32_t BLOCK_SIZE = 1 << HANDLE_OFFSET_BITS;
		static constexpr uint32_t HANDLE_INDEX_BITS = 32 - HANDLE_OFFSET_BITS;
		static constexpr uint32_t HANDLE_INDEX_MASK = 0xffffffff & (0xffffffff << HANDLE_OFFSET_BITS);
		static constexpr uint32_t HANDLE_INDEX_OFFSET = HANDLE_OFFSET_BITS;

		static constexpr uint32_t ENTRY_ALIGNMENT_BITS = 1;
		static constexpr uint32_t ENTRY_ALIGNMENT = 1 << ENTRY_ALIGNMENT_BITS;
		static constexpr uint32_t ENTRY_ALIGNMENT_MASK = ~(ENTRY_ALIGNMENT - 1);

		// Hash class is used to calculate the hash value of a string.
		// The 64-bit hash value is divided into three parts: tag, index, and offset.
		// - tag: Used for a quick comparison before comparing characters individually.
		// - index: Determines which Pool the string's information will be stored in.
		// - offset: Determines which Slot in the Pool the string's infomation will be stored in.
		class Hash
		{
		public:
			Hash() = default;

			Hash(uint64_t hash)
				: hash(hash)
			{
			}

			// Constructor for Hash that takes a const char* and its size.
			// Calculates the 64-bit hash value for the given string.
			Hash(const char* str, uint16_t size);

			constexpr uint32_t getIndex() const { return (address & HASH_INDEX_MASK) >> HASH_INDEX_OFFSET; }
			constexpr uint32_t getOffset() const { return address & HASH_OFFSET_MASK; }
			constexpr uint32_t getTag() const { return tag; }

		private:
			union
			{
				uint64_t hash;
				struct
				{
					uint32_t address;					// Combined index and offset.
					uint32_t tag;
				};
			};
		};

		// Entry represents the actual storage for the content of strings.
		// Entry retains only the length data; it is always followed by a variable-length string.
		// | size(16) | data(...) | next_size(16) | next_data(...) | ...
		class Entry
		{
		public:
			uint16_t getSize() const { return size; }
			const char* getData() const { return (const char*)(this + 1); }

			// Set the Entry data
			void set(const char* str, uint16_t size);
		private:
			uint16_t size;
		};

		// EntryHandle class is used to locate the position of a string within the blocks owned by EntryAllocator,
		// which Identifier holds.
		class EntryHandle
		{
		public:
			// Default constructor for EntryHandle.
			EntryHandle() = default;

			// Constructor for EntryHandle that takes index and offset.
			EntryHandle(uint32_t index, uint32_t offset);

			uint32_t getIndex() const { return (handle & HANDLE_INDEX_MASK) >> HANDLE_INDEX_OFFSET; }
			uint32_t getOffset() const { return handle & (HANDLE_OFFSET_MASK & ENTRY_ALIGNMENT_MASK); }

			// Checks if the handle is marked as used.
			bool used() const { return handle & USED_MASK; }

			operator uint32_t() const { return handle; }
		private:
			// 32-bit handle containing the following components:
			// - Bits 31 to 12: Block index (20 bits).
			// - Bits 11 to 0: Offset within the block (12 bits, with the last bit always 0 for 2-byte alignment).
			// - Bit 0 : Used flag (1 if used, 0 if not used).
			uint32_t handle = 0;
		};

		// Slot class stores summarized information related to strings,
		// indicating whether the string is already stored.
		class Slot
		{
		public:
			// Default constructor for Slot.
			Slot() = default;

			// Returns the handle associated with the slot.
			EntryHandle& getHandle() { return handle; }

			// Sets the tag and handle for the slot.
			void set(uint32_t tag, const EntryHandle handle);

			// Checks if the slot is marked as used.
			bool used() const { return handle.used(); }

			// Checks if the slot contains a specific string based on tag, size, and content.
			bool contain(const char* str, uint16_t size, uint32_t tag) const;
		private:
			uint32_t tag;
			EntryHandle handle;
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
			uint32_t count;								// Number of used slots in the pool.
			Slot* slots;
			std::shared_mutex rwMutex;
		};

		// EntryAllocator class is responsible for allocating new Entry instances and mapping EntryHandle to Entry.
		// It is implemented as a singleton.
		class EntryAllocator
		{
		public:
			// Destructor for EntryAllocator.
			~EntryAllocator();

			EntryAllocator(const EntryAllocator&) = delete;
			EntryAllocator& operator=(const EntryAllocator&) = delete;

			// Gets the Entry associated with a given EntryHandle.
			static Entry* getEntry(EntryHandle handle) { return instance.getEntryImpl(handle); }

			// Acquires a new Entry for a given string and information.
			static EntryHandle acquireEntry(const char* str, uint16_t size) { return instance.acquireEntryImpl(str, size); }

			static EntryHandle acquireEntry(const char* str, uint16_t size, Hash hash) { return instance.acquireEntryImpl(str, size, hash); }
		private:
			// Default constructor for EntryAllocator.
			EntryAllocator();

			// Acquires a block of memory with a size of 'size'.
			EntryHandle acquireMemory(uint16_t size);

			// Gets the Entry associated with a given EntryHandle.
			Entry* getEntryImpl(EntryHandle handle) const
			{
				return handle.used() ? (Entry*)(blocks[handle.getIndex()] + (handle.getOffset())) : nullptr;
			}

			// Acquires a new Entry for a given string and information.
			EntryHandle acquireEntryImpl(const char* str, uint16_t size);
			EntryHandle acquireEntryImpl(const char* str, uint16_t size, Hash hash);
		private:
			Bytes** blocks;
			uint32_t capacity;

			uint32_t blockIndex;
			uint32_t blockOffset;

			Pool pools[HASH_INDEX_SPACE];

			std::mutex mutex;

			static EntryAllocator instance;					// Global unique EntryAllocator.
		};
	private:
		friend struct std::hash<Pache::Identifier>;
		EntryHandle id;
	};
}

template <>
struct std::hash<Pache::Identifier>
{
	std::size_t operator()(const Pache::Identifier& identifier) const
	{
		return hash<uint32_t>()(identifier.id);
	}
};

template <>
struct fmt::formatter<Pache::Identifier>
	: fmt::formatter<const char*>
{
	template <typename FormatCtx>
	auto format(const Pache::Identifier& id, FormatCtx& ctx) const
	{
		return fmt::format_to(ctx.out(), "{}", id.c_str());
	}
};
