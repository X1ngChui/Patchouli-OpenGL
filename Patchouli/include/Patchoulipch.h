#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <concepts>
#include <type_traits>
#include <memory>
#include <mutex>
#include <utility>
#include <algorithm>
#include <functional>
#include <thread>
#include <iterator>
#include <compare>

#include <atomic>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>

#include "Core/Base.h"
#include "Core/Log.h"
 
#include "Identifier.h"
#include "ResourcePtr.h"

#ifdef _WIN64
#include <Windows.h>
#endif

/*
#ifdef _MSC_VER
	#if defined(_M_X64) || defined(_M_AMD64)
#define PACHE_SIMD_SUPPORTED
#include <immintrin.h>
	#endif
#elif defined(__GNUC__)
	#if defined(__x86_64__) || defined(__amd64__)
#define PACHE_SIMD_SUPPORTED
#include <immintrin.h>
	#endif
#endif
*/