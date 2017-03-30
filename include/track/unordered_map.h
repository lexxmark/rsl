#pragma once

#ifndef TRACK_UNORDERED_MAP_H
#define TRACK_UNORDERED_MAP_H

#include "allocator.h"
#include "iterator.h"
#include <unordered_map>

namespace rsl
{
	namespace track
	{
        template <class K, class T, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<std::pair<const K, T>>>
        using unordered_map = std::unordered_map<K, T, Hasher, Keyeq, allocator<std::pair<const K, T>, Alloc>>;

        template <class K, class T, class Hasher = std::hash<K>, class Keyeq = std::equal_to<K>, class Alloc = std::allocator<std::pair<const K, T>>>
        const auto& get_trackable(const unordered_map<K, T, Hasher, Keyeq, Alloc>& map)
        {
            return map.get_allocator().get_trackable();
        }
	} // end namespace track
} // end namespace rsl

#endif // TRACK_UNORDERED_MAP_H
