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
	auto get_cref(const unordered_set_ref<K, Hasher, Keyeq, Alloc>& set, const K& key)
	{
		auto it = set.find(key);
		if (it == set.cend())
			return ref_ptr<const K>();

		auto alloc = set.get_allocator();
		return make_ref(&(*it), alloc.get_trackable());
	}

	template <class K, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<K>>
	auto get_ref(unordered_set_ref<K, Hasher, Keyeq, Alloc>& set, const K& key)
	{
		auto it = set.find(key);
		if (it == set.end())
			return ref_ptr<const K>();

		auto alloc = set.get_allocator();
		return make_ref<const K>(&(*it), alloc.get_trackable());
	}
} // end namespace rsl

#endif // REF_UNORDERED_SET_H
