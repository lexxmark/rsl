#pragma once

#ifndef REF_UNORDERED_SET_H
#define REF_UNORDERED_SET_H

#include "ref_allocator.h"
#include <unordered_set>

namespace rsl
{
	template <class K, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<K>>
	using unordered_set_ref = std::unordered_set<K, Hasher, Keyeq, ref_allocator<K, Alloc>>;

	template <class K, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<K>>
	const auto& get_trackable(const unordered_set_ref<K, Hasher, Keyeq, Alloc>& set)
	{
		return set.get_allocator().get_trackable();
	}

} // end namespace rsl

#endif // REF_UNORDERED_SET_H
