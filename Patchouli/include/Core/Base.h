#pragma once
#include "ResourcePtr.h"

namespace Pache
{
	template <typename T>
	using Scope = UniquePtr<T>;

	template <typename T>
	using Ref = IntrusivePtr<T>;
}