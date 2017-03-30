#pragma once

#ifndef TRACK_SET_H
#define TRACK_SET_H

#include "allocator.h"
#include "iterator.h"
#include <set>

namespace rsl
{
	namespace track
	{
        template <class K, class Pr = std::less<K>, class Alloc = std::allocator<K>>
        using set = std::set<K, Pr, allocator<K, Alloc>>;

        template <class K, class Pr = std::less<K>, class Alloc = std::allocator<K>>
        const trackable& get_trackable(const set<K, Pr, Alloc>& s)
        {
            return s.get_allocator().get_trackable();
        }
	} // end namespace track
} // end namespace rsl

#endif // TRACK_SET_H
