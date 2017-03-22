#pragma once

#ifndef REF_UNORDERED_MAP_H
#define REF_UNORDERED_MAP_H

#include "ref_allocator.h"
#include <unordered_map>

namespace rsl
{
	template <class K, class T, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<std::pair<const K, T>>>
	using unordered_map_ref = std::unordered_map<K, T, Hasher, Keyeq, ref_allocator<std::pair<const K, T>, Alloc>>;

	template <class K, class T, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<std::pair<const K, T>>>
	const auto& get_trackable(const unordered_map_ref<K, T, Hasher, Keyeq, Alloc>& map)
	{
		return map.get_allocator().get_trackable();
	}

} // end namespace rsl

#endif // REF_UNORDERED_MAP_H
