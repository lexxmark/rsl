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
	const auto& get_trackable(const set_ref<K, Pr, Alloc>& set)
	{
		return set.get_allocator().get_trackable();
	}

} // end namespace rsl

#endif // REF_SET_H
