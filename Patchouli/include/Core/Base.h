#pragma once
#include "ResourcePtr.h"

namespace Pache
{
	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T>
	using Ref = IntrusivePtr<T>;
}