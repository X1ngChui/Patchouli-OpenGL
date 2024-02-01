#pragma once
#include "ResourcePtr.h"
#include "mimalloc.h"

inline void* operator new(size_t size)
{
	return mi_malloc(size);
}

inline void operator delete(void* ptr) noexcept
{
	mi_free(ptr);
}

inline void* operator new[](size_t size)
{
	return mi_malloc(size);
}

inline void operator delete[](void* ptr) noexcept
{
	mi_free(ptr);
}

namespace Pache
{
	template <typename T>
	using Scope = UniquePtr<T>;

	template <typename T>
	using Ref = IntrusivePtr<T>;
}