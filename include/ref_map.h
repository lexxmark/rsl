#pragma once

#ifndef REF_MAP_H
#define REF_MAP_H

#include "ref_allocator.h"
#include <map>

namespace rsl
{
	template <class K, class T, class Pr = std::less<K>, class Alloc = std::allocator<pair<const K, T>>>
	using map_ref = std::map<K, T, Pr, ref_allocator<pair<const K, T>, Alloc>>;

	template <class K, class T, class Pr = std::less<K>, class Alloc = std::allocator<pair<const K, T>>>
	auto get_cref(const map_ref<K, T, Pr, Alloc>& map, const K& key)
	{
		auto it = map.find(key);
		if (it == map.cend())
			return ref_ptr<const T>();

		auto alloc = map.get_allocator();
		return make_ref(&(*it).second, alloc.get_trackable());
	}

	template <class K, class T, class Pr = std::less<K>, class Alloc = std::allocator<pair<const K, T>>>
	auto get_ref(map_ref<K, T, Pr, Alloc>& map, const K& key)
	{
		auto it = map.find(key);
		if (it == map.end())
			return ref_ptr<T>();

		auto alloc = map.get_allocator();
		return make_ref<T>(&(*it).second, alloc.get_trackable());
	}
} // end namespace rsl

#endif // REF_MAP_H
