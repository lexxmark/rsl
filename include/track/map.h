#pragma once

#ifndef TRACK_MAP_H
#define TRACK_MAP_H

#include "allocator.h"
#include "iterator.h"
#include <map>

namespace rsl
{
	namespace track
	{
        template <class K, class T, class Pr = std::less<K>, class Alloc = std::allocator<std::pair<const K, T>>>
        using map = std::map<K, T, Pr, allocator<std::pair<const K, T>, Alloc>>;

        template <class K, class T, class Pr = std::less<K>, class Alloc = std::allocator<std::pair<const K, T>>>
        const trackable& get_trackable(const map<K, T, Pr, Alloc>& m)
        {
            return m.get_allocator().get_trackable();
        }
	} // end namespace track
} // end namespace rsl

#endif // TRACK_MAP_H
