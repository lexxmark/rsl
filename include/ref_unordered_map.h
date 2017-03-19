#pragma once

#ifndef REF_UNORDERED_MAP_H
#define REF_UNORDERED_MAP_H

#include "ref_allocator.h"
#include <unordered_map>

namespace rsl
{
	template <class K, class T, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<pair<const K, T>>>
	using unordered_map_ref = std::unordered_map<K, T, Hasher, Keyeq, ref_allocator<pair<const K, T>, Alloc>>;

	template <class K, class T, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<pair<const K, T>>>
	auto get_cref(const unordered_map_ref<K, T, Hasher, Keyeq, Alloc>& map, const K& key)
	{
		auto it = map.find(key);
		if (it == map.cend())
			return ref_ptr<const T>();

		auto alloc = map.get_allocator();
		return make_ref(&(*it).second, alloc.get_trackable());
	}

	template <class K, class T, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<pair<const K, T>>>
	auto get_ref(unordered_map_ref<K, T, Hasher, Keyeq, Alloc>& map, const K& key)
	{
		auto it = map.find(key);
		if (it == map.end())
			return ref_ptr<T>();

		auto alloc = map.get_allocator();
		return make_ref<T>(&(*it).second, alloc.get_trackable());
	}
} // end namespace rsl

#endif // REF_UNORDERED_MAP_H
