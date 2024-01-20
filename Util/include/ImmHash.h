// The following code is based on XXH3_64bits from xxhash, modified to create
// a platform-independent, compile-time calculable version.
// 
// In the process of modification, optimizing calculation speed is not the primary
// concern; this code is designed to provide compile-time support.
// 
// Note: XXH3_64bits is a hash algorithm from the xxhash library, commonly used for
// fast runtime hash calculations. The Identifier::Hash also utilizes this algorithm.
// 
// This modified version focuses on hash calculation during compile-time, rather than
// pursuing runtime performance.
// 
// Please be aware that subtle differences may exist across platforms; it is advisable
// to carefully read the documentation and conduct testing to ensure expectations are met.

#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>

#ifdef _WIN64
	#define CPU_LITTLE_ENDIAN 1
#endif

namespace ImmHash
{
	static constexpr uint64_t PRIME_MX1 = 0x165667919E3779F9ULL;  /*!< 0b0001011001010110011001111001000110011110001101110111100111111001 */
	static constexpr uint64_t PRIME_MX2 = 0x9FB21C651E98DF25ULL;  /*!< 0b1001111110110010000111000110010100011110100110001101111100100101 */

	static constexpr uint32_t PRIME32_1 = 0x9E3779B1U;  /*!< 0b10011110001101110111100110110001 */
	static constexpr uint32_t PRIME32_2 = 0x85EBCA77U;  /*!< 0b10000101111010111100101001110111 */
	static constexpr uint32_t PRIME32_3 = 0xC2B2AE3DU;  /*!< 0b11000010101100101010111000111101 */
	static constexpr uint32_t PRIME32_4 = 0x27D4EB2FU;  /*!< 0b00100111110101001110101100101111 */
	static constexpr uint32_t PRIME32_5 = 0x165667B1U; /*!< 0b00010110010101100110011110110001 */
	static constexpr uint64_t PRIME64_1 = 0x9E3779B185EBCA87ULL;  /*!< 0b1001111000110111011110011011000110000101111010111100101010000111 */
	static constexpr uint64_t PRIME64_2 = 0xC2B2AE3D27D4EB4FULL;  /*!< 0b1100001010110010101011100011110100100111110101001110101101001111 */
	static constexpr uint64_t PRIME64_3 = 0x165667B19E3779F9ULL;  /*!< 0b0001011001010110011001111011000110011110001101110111100111111001 */
	static constexpr uint64_t PRIME64_4 = 0x85EBCA77C2B2AE63ULL;  /*!< 0b1000010111101011110010100111011111000010101100101010111001100011 */
	static constexpr uint64_t PRIME64_5 = 0x27D4EB2F165667C5ULL;  /*!< 0b0010011111010100111010110010111100010110010101100110011111000101 */

	static constexpr size_t SECRET_SIZE_MIN = 136;
	static constexpr size_t SECRET_DEFAULT_SIZE = 192;
	alignas(64) static constexpr uint8_t kSecret[SECRET_DEFAULT_SIZE] = {
		0xb8, 0xfe, 0x6c, 0x39, 0x23, 0xa4, 0x4b, 0xbe, 0x7c, 0x01, 0x81, 0x2c, 0xf7, 0x21, 0xad, 0x1c,
		0xde, 0xd4, 0x6d, 0xe9, 0x83, 0x90, 0x97, 0xdb, 0x72, 0x40, 0xa4, 0xa4, 0xb7, 0xb3, 0x67, 0x1f,
		0xcb, 0x79, 0xe6, 0x4e, 0xcc, 0xc0, 0xe5, 0x78, 0x82, 0x5a, 0xd0, 0x7d, 0xcc, 0xff, 0x72, 0x21,
		0xb8, 0x08, 0x46, 0x74, 0xf7, 0x43, 0x24, 0x8e, 0xe0, 0x35, 0x90, 0xe6, 0x81, 0x3a, 0x26, 0x4c,
		0x3c, 0x28, 0x52, 0xbb, 0x91, 0xc3, 0x00, 0xcb, 0x88, 0xd0, 0x65, 0x8b, 0x1b, 0x53, 0x2e, 0xa3,
		0x71, 0x64, 0x48, 0x97, 0xa2, 0x0d, 0xf9, 0x4e, 0x38, 0x19, 0xef, 0x46, 0xa9, 0xde, 0xac, 0xd8,
		0xa8, 0xfa, 0x76, 0x3f, 0xe3, 0x9c, 0x34, 0x3f, 0xf9, 0xdc, 0xbb, 0xc7, 0xc7, 0x0b, 0x4f, 0x1d,
		0x8a, 0x51, 0xe0, 0x4b, 0xcd, 0xb4, 0x59, 0x31, 0xc8, 0x9f, 0x7e, 0xc9, 0xd9, 0x78, 0x73, 0x64,
		0xea, 0xc5, 0xac, 0x83, 0x34, 0xd3, 0xeb, 0xc3, 0xc5, 0x81, 0xa0, 0xff, 0xfa, 0x13, 0x63, 0xeb,
		0x17, 0x0d, 0xdd, 0x51, 0xb7, 0xf0, 0xda, 0x49, 0xd3, 0x16, 0x55, 0x26, 0x29, 0xd4, 0x68, 0x9e,
		0x2b, 0x16, 0xbe, 0x58, 0x7d, 0x47, 0xa1, 0xfc, 0x8f, 0xf8, 0xb8, 0xd1, 0x7a, 0xd0, 0x31, 0xce,
		0x45, 0xcb, 0x3a, 0x8f, 0x95, 0x16, 0x04, 0x28, 0xaf, 0xd7, 0xfb, 0xca, 0xbb, 0x4b, 0x40, 0x7e
	};

	static constexpr size_t MIDSIZE_MAX = 240;
	static constexpr size_t STRIPE_LEN = 64;
	static constexpr size_t SECRET_CONSUME_RATE = 8;

	static constexpr size_t ACC_ALIGN = 8;
	static constexpr size_t ACC_NB = STRIPE_LEN / sizeof(uint64_t);

	typedef struct {
		uint64_t low64;   /*!< `value & 0xFFFFFFFFFFFFFFFF` */
		uint64_t high64;  /*!< `value >> 64` */
	} uint128_t;

	template <size_t N, size_t S>
	using hashLong64_f = uint64_t(*)(const char(&)[N], uint64_t, const uint8_t(&)[S]);

	template <size_t A, size_t N, size_t S>
	using f_accumulate = void (*)(uint64_t(&)[A], const char(&)[N], size_t, const uint8_t(&)[S], size_t);

	template <size_t A, size_t S>
	using f_scrambleAcc = void(*)(uint64_t(&)[A], const uint8_t(&)[S], size_t);

	static inline consteval uint64_t swap64(uint64_t x)
	{
		return ((x << 56) & 0xff00000000000000ULL) |
			((x << 40) & 0x00ff000000000000ULL) |
			((x << 24) & 0x0000ff0000000000ULL) |
			((x << 8) & 0x000000ff00000000ULL) |
			((x >> 8) & 0x00000000ff000000ULL) |
			((x >> 24) & 0x0000000000ff0000ULL) |
			((x >> 40) & 0x000000000000ff00ULL) |
			((x >> 56) & 0x00000000000000ffULL);
	}

	static inline consteval uint32_t swap32(uint32_t x)
	{
		return  ((x << 24) & 0xff000000) |
			((x << 8) & 0x00ff0000) |
			((x >> 8) & 0x0000ff00) |
			((x >> 24) & 0x000000ff);
	}

	template <size_t L>
	static consteval uint64_t read64(const char(&mem)[L], size_t offset = 0)
	{
		uint64_t val = 0;
		val |= (uint64_t)mem[0 + offset];
		val |= (uint64_t)mem[1 + offset] << 8;
		val |= (uint64_t)mem[2 + offset] << 16;
		val |= (uint64_t)mem[3 + offset] << 24;
		val |= (uint64_t)mem[4 + offset] << 32;
		val |= (uint64_t)mem[5 + offset] << 40;
		val |= (uint64_t)mem[6 + offset] << 48;
		val |= (uint64_t)mem[7 + offset] << 56;
		return val;
	}

	template <size_t L>
	static consteval uint64_t read64(const uint8_t(&mem)[L], size_t offset = 0)
	{
		uint64_t val = 0;
		val |= (uint64_t)mem[0 + offset];
		val |= (uint64_t)mem[1 + offset] << 8;
		val |= (uint64_t)mem[2 + offset] << 16;
		val |= (uint64_t)mem[3 + offset] << 24;
		val |= (uint64_t)mem[4 + offset] << 32;
		val |= (uint64_t)mem[5 + offset] << 40;
		val |= (uint64_t)mem[6 + offset] << 48;
		val |= (uint64_t)mem[7 + offset] << 56;
		return val;
	}

	template <size_t L>
	static consteval uint32_t read32(const char(&mem)[L], size_t offset = 0)
	{
		uint32_t val = 0;
		val |= (uint32_t)mem[0 + offset];
		val |= (uint32_t)mem[1 + offset] << 8;
		val |= (uint32_t)mem[2 + offset] << 16;
		val |= (uint32_t)mem[3 + offset] << 24;
		return val;
	}

	template <size_t L>
	static consteval uint32_t read32(const uint8_t(&mem)[L], size_t offset = 0)
	{
		uint32_t val = 0;
		val |= (uint32_t)mem[0 + offset];
		val |= (uint32_t)mem[1 + offset] << 8;
		val |= (uint32_t)mem[2 + offset] << 16;
		val |= (uint32_t)mem[3 + offset] << 24;
		return val;
	}

	template <size_t L>
	static inline consteval uint64_t readLE64(const char(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? read64(ptr, offset) : swap64(read64(ptr, offset));
	}

	template <size_t L>
	static inline consteval uint64_t readLE64(const uint8_t(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? read64(ptr, offset) : swap64(read64(ptr, offset));
	}

	template <size_t L>
	static consteval uint64_t readBE64(const char(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? swap64(read64(ptr, offset)) : read64(ptr, offset);
	}

	template <size_t L>
	static consteval uint64_t readBE64(const uint8_t(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? swap64(read64(ptr, offset)) : read64(ptr, offset);
	}

	template <size_t L>
	static inline consteval uint32_t readLE32(const char(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? read32(ptr, offset) : swap32(read32(ptr, offset));
	}

	template <size_t L>
	static inline consteval uint32_t readLE32(const uint8_t(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? read32(ptr, offset) : swap32(read32(ptr, offset));
	}

	template <size_t L>
	static consteval uint32_t readBE32(const char(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? swap32(read32(ptr, offset)) : read32(ptr, offset);
	}

	template <size_t L>
	static consteval uint32_t readBE32(const uint8_t(&ptr)[L], size_t offset = 0)
	{
		return CPU_LITTLE_ENDIAN ? swap32(read32(ptr, offset)) : read32(ptr, offset);
	}

	static inline consteval uint64_t mult32to64(uint32_t x, uint32_t y)
	{
		return ((uint64_t)(x) * (uint64_t)(y));
	}

	static inline consteval uint32_t rotl32(uint32_t x, int r)
	{
		return ((x) << (r)) | ((x) >> (32 - (r)));
	}

	static inline consteval uint64_t rotl64(uint64_t x, int r)
	{
		return ((x) << (r)) | ((x) >> (64 - (r)));
	}

	static consteval uint128_t
		mult64to128(uint64_t lhs, uint64_t rhs)
	{
		uint64_t const lo_lo = mult32to64(lhs & 0xFFFFFFFF, rhs & 0xFFFFFFFF);
		uint64_t const hi_lo = mult32to64(lhs >> 32, rhs & 0xFFFFFFFF);
		uint64_t const lo_hi = mult32to64(lhs & 0xFFFFFFFF, rhs >> 32);
		uint64_t const hi_hi = mult32to64(lhs >> 32, rhs >> 32);

		/* Now add the products together. These will never overflow. */
		uint64_t const cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
		uint64_t const upper = (hi_lo >> 32) + (cross >> 32) + hi_hi;
		uint64_t const lower = (cross << 32) | (lo_lo & 0xFFFFFFFF);

		uint128_t r128;
		r128.low64 = lower;
		r128.high64 = upper;
		return r128;
	}


	static consteval uint64_t mul128_fold64(uint64_t lhs, uint64_t rhs)
	{
		uint128_t product = mult64to128(lhs, rhs);
		return product.low64 ^ product.high64;
	}

	static inline consteval uint64_t xorshift64(uint64_t v64, int shift)
	{
		return v64 ^ (v64 >> shift);
	}

	// XXH3_avalanche
	static consteval uint64_t avalanche(uint64_t h64)
	{
		h64 = xorshift64(h64, 37);
		h64 *= PRIME_MX1;
		h64 = xorshift64(h64, 32);
		return h64;
	}

	// XXH64_avalanche
	static consteval uint64_t avalanche64(uint64_t hash)
	{
		hash ^= hash >> 33;
		hash *= PRIME64_2;
		hash ^= hash >> 29;
		hash *= PRIME64_3;
		hash ^= hash >> 32;
		return hash;
	}

	template <size_t N, size_t S>
	static inline consteval uint64_t
		len_9to16_64b(const char(&input)[N], const uint8_t(&secret)[S], uint64_t seed)
	{
		constexpr size_t len = N - 1;

		uint64_t const bitflip1 = (readLE64(secret, 24) ^ readLE64(secret, 32)) + seed;
		uint64_t const bitflip2 = (readLE64(secret, 40) ^ readLE64(secret, 48)) - seed;
		uint64_t const input_lo = readLE64(input) ^ bitflip1;
		uint64_t const input_hi = readLE64(input, len - 8) ^ bitflip2;
		uint64_t const acc = len
			+ swap64(input_lo) + input_hi
			+ mul128_fold64(input_lo, input_hi);
		return avalanche(acc);
	}

	template <size_t N, size_t S>
	static inline consteval uint64_t mix16B(const char(&input)[N], size_t inputOffset,
		const uint8_t(&secret)[S], size_t secretOffset, uint64_t seed64)
	{
		uint64_t const input_lo = readLE64(input, inputOffset);
		uint64_t const input_hi = readLE64(input, inputOffset + 8);
		return mul128_fold64(
			input_lo ^ (readLE64(secret, secretOffset) + seed64),
			input_hi ^ (readLE64(secret, secretOffset + 8) - seed64)
		);
	}

	static consteval uint64_t rrmxmx(uint64_t h64, uint64_t len)
	{
		/* this mix is inspired by Pelle Evensen's rrmxmx */
		h64 ^= rotl64(h64, 49) ^ rotl64(h64, 24);
		h64 *= PRIME_MX2;
		h64 ^= (h64 >> 35) + len;
		h64 *= PRIME_MX2;
		return xorshift64(h64, 28);
	}

	template <size_t N, size_t S>
	static inline consteval uint64_t len_4to8_64b(const char(&input)[N], const uint8_t(&secret)[S], uint64_t seed)
	{
		constexpr size_t len = N - 1;

		seed ^= (uint64_t)swap32((uint32_t)seed) << 32;
		uint32_t const input1 = readLE32(input);
		uint32_t const input2 = readLE32(input, len - 4);
		uint64_t const bitflip = (readLE64(secret, 8) ^ readLE64(secret, 16)) - seed;
		uint64_t const input64 = input2 + (((uint64_t)input1) << 32);
		uint64_t const keyed = input64 ^ bitflip;
		return rrmxmx(keyed, len);

	}

	/* For mid range keys, XXH3 uses a Mum-hash variant. */
	template <size_t N, size_t S>
	static inline consteval uint64_t len_17to128_64b(const char(&input)[N], const uint8_t(&secret)[S], uint64_t seed)
	{
		constexpr size_t len = N - 1;

		uint64_t acc = len * PRIME64_1;
		if (len > 32) {
			if (len > 64) {
				if (len > 96) {
					acc += mix16B(input, 48, secret, 96, seed);
					acc += mix16B(input, len - 64, secret, 112, seed);
				}
				acc += mix16B(input, 32, secret, 64, seed);
				acc += mix16B(input, len - 48, secret, 80, seed);
			}
			acc += mix16B(input, 16, secret, 32, seed);
			acc += mix16B(input, len - 32, secret, 48, seed);
		}
		acc += mix16B(input, 0, secret, 0, seed);
		acc += mix16B(input, len - 16, secret, 16, seed);

		return avalanche(acc);
	}

	template <size_t N, size_t S>
	static inline consteval uint64_t len_1to3_64b(const char(&input)[N], const uint8_t(&secret)[S], uint64_t seed)
	{
		/*
		 * len = 1: combined = { input[0], 0x01, input[0], input[0] }
		 * len = 2: combined = { input[1], 0x02, input[0], input[1] }
		 * len = 3: combined = { input[2], 0x03, input[0], input[1] }
		 */
		constexpr size_t len = N - 1;

		uint8_t const c1 = input[0];
		uint8_t const c2 = input[len >> 1];
		uint8_t const c3 = input[len - 1];
		uint32_t const combined = ((uint32_t)c1 << 16) | ((uint32_t)c2 << 24)
			| ((uint32_t)c3 << 0) | ((uint32_t)len << 8);
		uint64_t const bitflip = (readLE32(secret) ^ readLE32(secret, 4)) + seed;
		uint64_t const keyed = (uint64_t)combined ^ bitflip;
		return avalanche64(keyed);
	}

	template <size_t N, size_t S>
	static inline consteval uint64_t
		len_0to16_64b(const char(&input)[N], const uint8_t(&secret)[S], uint64_t seed)
	{
		constexpr size_t len = N - 1;

		if (len > 8)	return len_9to16_64b(input, secret, seed);
		if (len >= 4)	return len_4to8_64b(input, secret, seed);
		if (len)				return len_1to3_64b(input, secret, seed);
		return avalanche64(seed ^ (readLE64(secret, 56) ^ readLE64(secret, 64)));
	}

	template <size_t N, size_t S>
	static consteval uint64_t len_129to240_64b(const char(&input)[N], const uint8_t(&secret)[S], uint64_t seed)
	{
		constexpr size_t len = N - 1;

		constexpr size_t MIDSIZE_STARTOFFSET = 3;
		constexpr size_t MIDSIZE_LASTOFFSET = 17;

		uint64_t acc = len * PRIME64_1;
		uint64_t acc_end;
		unsigned int const nbRounds = (unsigned int)len / 16;
		unsigned int i;
		for (i = 0; i < 8; i++) {
			acc += mix16B(input, 16 * i, secret, 16 * i, seed);
		}
		/* last bytes */
		acc_end = mix16B(input, len - 16, secret, SECRET_SIZE_MIN - MIDSIZE_LASTOFFSET, seed);
		acc = avalanche(acc);

		for (i = 8; i < nbRounds; i++) {
			/*
			 * Prevents clang for unrolling the acc loop and interleaving with this one.
			 */
			acc_end += mix16B(input, 16 * i, secret, 16 * (i - 8) + MIDSIZE_STARTOFFSET, seed);
		}
		return avalanche(acc + acc_end);
	}

	static inline consteval uint64_t mult32to64_add64(uint64_t lhs, uint64_t rhs, uint64_t acc)
	{
		return mult32to64((uint32_t)lhs, (uint32_t)rhs) + acc;
	}

	template <size_t A, size_t N, size_t S>
	inline static consteval void round(uint64_t(&acc)[A],
		const char(&input)[N], size_t inputOffset,
		const uint8_t(&secret)[S], size_t secretOffset,
		size_t lane)
	{
		uint64_t const data_val = readLE64(input, inputOffset + lane * 8);
		uint64_t const data_key = data_val ^ readLE64(secret, secretOffset + lane * 8);
		acc[lane ^ 1] += data_val; /* swap adjacent lanes */
		acc[lane] = mult32to64_add64(data_key /* & 0xFFFFFFFF */, data_key >> 32, acc[lane]);
	}

	template <size_t A, size_t N, size_t S>
	static inline consteval void accumulate_512(uint64_t(&acc)[A], const char(&input)[N], size_t inputOffset,
		const uint8_t(&secret)[S], size_t secretOffset)
	{
		size_t i;
		for (i = 0; i < ACC_NB; i++) {
			round(acc, input, inputOffset, secret, secretOffset, i);
		}
	}

	template <size_t A, size_t N, size_t S>
	static inline consteval void accumulate(uint64_t(&acc)[A],
		const char(&input)[N], size_t inputOffset,
		const uint8_t(&secret)[S],
		size_t nbStripes)
	{
		size_t n;
		for (n = 0; n < nbStripes; n++) {
			accumulate_512(acc, input, inputOffset + n * STRIPE_LEN, secret, n * SECRET_CONSUME_RATE);
		}
	}

	template <size_t A, size_t S>
	static inline consteval void scrambleRound(uint64_t(&acc)[A], const uint8_t(&secret)[S], size_t lane)
	{

		uint64_t const key64 = readLE64(secret, lane * 8);
		uint64_t acc64 = acc[lane];
		acc64 = xorshift64(acc64, 47);
		acc64 ^= key64;
		acc64 *= PRIME32_1;
		acc[lane] = acc64;
	}

	template <size_t A, size_t S>
	static inline consteval void scrambleAcc(uint64_t(&acc)[A], const uint8_t(&secret)[S], size_t secretOffset)
	{
		size_t i;
		for (i = 0; i < ACC_NB; i++) {
			scrambleRound(acc, secret, i);
		}
	}

	template<size_t N, size_t S>
	static inline consteval uint64_t hash_internal(const char(&input)[N], uint64_t seed64,
		const uint8_t(&secret)[S], hashLong64_f<N, S> f_hashLong)
	{
		constexpr size_t len = N - 1;
		constexpr size_t secretLen = S - 1;

		if (len <= 16)
			return len_0to16_64b(input, secret, seed64);
		if (len <= 128)
			return len_17to128_64b(input, secret, seed64);
		if (len <= MIDSIZE_MAX)
			return len_129to240_64b(input, secret, seed64);
		return f_hashLong(input, seed64, secret);
	}

	template <size_t A, size_t N, size_t S>
	static inline consteval void hashLong_internal_loop(uint64_t(&acc)[A],
		const char(&input)[N], const uint8_t(&secret)[S],
		f_accumulate<A, N, S> f_acc,
		f_scrambleAcc<A, S> f_scramble)
	{
		constexpr size_t len = N - 1;
		constexpr size_t secretSize = S;

		size_t const nbStripesPerBlock = (secretSize - STRIPE_LEN) / SECRET_CONSUME_RATE;
		size_t const block_len = STRIPE_LEN * nbStripesPerBlock;
		size_t const nb_blocks = (len - 1) / block_len;

		size_t n;
		for (n = 0; n < nb_blocks; n++) {
			f_acc(acc, input, n * block_len, secret, nbStripesPerBlock);
			f_scramble(acc, secret, secretSize - STRIPE_LEN);
		}

		/* last partial block */
		size_t const nbStripes = ((len - 1) - (block_len * nb_blocks)) / STRIPE_LEN;
		f_acc(acc, input, nb_blocks * block_len, secret, nbStripes);

		/* last stripe */
		constexpr size_t SECRET_LASTACC_START = 7;  /* not aligned on 8, last secret is different from acc & scrambler */
		accumulate_512(acc, input, len - STRIPE_LEN, secret, secretSize - STRIPE_LEN - SECRET_LASTACC_START);
	}

	template <size_t A, size_t S>
	static inline consteval uint64_t mix2Accs(const uint64_t(&acc)[A], size_t accOffset,
		const uint8_t(&secret)[S], size_t secretOffset)
	{
		return mul128_fold64(
			acc[0 + accOffset] ^ readLE64(secret, secretOffset),
			acc[1 + accOffset] ^ readLE64(secret, 8 + secretOffset));
	}

	template <size_t A, size_t S>
	static consteval uint64_t mergeAccs(const uint64_t(&acc)[A], const uint8_t(&secret)[S], size_t secretOffset, uint64_t start)
	{
		uint64_t result64 = start;
		size_t i = 0;

		for (i = 0; i < 4; i++) {
			result64 += mix2Accs(acc, 2 * i, secret, secretOffset + 16 * i);
		}

		return avalanche(result64);
	}

	template <size_t A, size_t N, size_t S>
	static inline consteval uint64_t hashLong_64b_internal(const char(&input)[N], const uint8_t(&secret)[S],
		f_accumulate<A, N, S> f_acc,
		f_scrambleAcc<A, S> f_scramble)
	{
		constexpr size_t len = N - 1;

		alignas(ACC_ALIGN) uint64_t acc[ACC_NB] = {
			PRIME32_3, PRIME64_1, PRIME64_2, PRIME64_3,
			PRIME64_4, PRIME32_2, PRIME64_5, PRIME32_1
		};

		hashLong_internal_loop<A, N, S>(acc, input, secret, f_acc, f_scramble);

		/* converge into final hash */
		/* do not align on 8, so that the secret is different from the accumulator */
		constexpr size_t SECRET_MERGEACCS_START = 11;
		return mergeAccs(acc, secret, SECRET_MERGEACCS_START, (uint64_t)len * PRIME64_1);
	}

	template <size_t N, size_t S>
	static consteval uint64_t hashLong_64b_default(const char(&input)[N], uint64_t seed64, const uint8_t(&secret)[S])
	{
		(void)seed64; (void)secret;
		return hashLong_64b_internal<ACC_NB, N, S>(input, kSecret,
			accumulate<ACC_NB, N, S>, scrambleAcc<ACC_NB, S>);
	}

	template<size_t N>
	inline consteval uint64_t hash(const char(&input)[N])
	{
		return hash_internal(input, 0, kSecret, hashLong_64b_default);
	}
}
