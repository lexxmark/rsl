#pragma once

#ifndef REF_MAP_H
#define REF_MAP_H

#include "ref_allocator.h"
#include <map>

namespace rsl
{
	template <class K, class T, class Pr = std::less<K>, class Alloc = std::allocator<std::pair<const K, T>>>
	using map_ref = std::map<K, T, Pr, ref_allocator<std::pair<const K, T>, Alloc>>;

	template <class K, class T, class Pr = std::less<K>, class Alloc = std::allocator<std::pair<const K, T>>>
	const auto& get_trackable(const map_ref<K, T, Pr, Alloc>& map)
	{
		return map.get_allocator().get_trackable();
	}
	
} // end namespace rsl

#endif // REF_MAP_H
