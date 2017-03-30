#pragma once

#ifndef TRACK_UNORDERED_SET_H
#define TRACK_UNORDERED_SET_H

#include "allocator.h"
#include "iterator.h"
#include <unordered_set>

namespace rsl
{
	namespace track
	{
		template <class K, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<K>>
		using unordered_set = std::unordered_set<K, Hasher, Keyeq, allocator<K, Alloc>>;

		template <class K, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<K>>
		const auto& get_trackable(const unordered_set<K, Hasher, Keyeq, Alloc>& set)
		{
			return set.get_allocator().get_trackable();
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_UNORDERED_SET_H
