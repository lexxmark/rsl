#pragma once

#ifndef REF_SET_H
#define REF_SET_H

#include "ref_allocator.h"
#include <set>

namespace rsl
{
	template <class K, class Pr = std::less<K>, class Alloc = std::allocator<K>>
	using set_ref = std::set<K, Pr, ref_allocator<K, Alloc>>;

	template <class K, class Pr = std::less<K>, class Alloc = std::allocator<K>>
	auto get_cref(const set_ref<K, Pr, Alloc>& set, const K& key)
	{
		auto it = set.find(key);
		if (it == set.cend())
			return ref_ptr<const K>();

		auto alloc = set.get_allocator();
		return make_ref(&(*it), alloc.get_trackable());
	}

	template <class K, class Pr = std::less<K>, class Alloc = std::allocator<K>>
	auto get_ref(set_ref<K, Pr, Alloc>& set, const K& key)
	{
		auto it = set.find(key);
		if (it == set.end())
			return ref_ptr<const K>();

		auto alloc = set.get_allocator();
		return make_ref<const K>(&(*it), alloc.get_trackable());
	}
} // end namespace rsl

#endif // REF_SET_H
