#pragma once

#ifndef TRACK_DEQUE_H
#define TRACK_DEQUE_H

#include "allocator.h"
#include "iterator.h"
#include "index.h"
#include <deque>

namespace rsl
{
	namespace track
	{
        template <typename T, typename Alloc = std::allocator<T>>
        using deque = std::deque<T, allocator<T, Alloc>>;

        template <class T, class Alloc = std::allocator<T>>
        const trackable& get_trackable(const deque<T, Alloc>& d)
        {
            return d.get_allocator().get_trackable();
        }
	} // end namespace track
} // end namespace rsl

#endif // TRACK_DEQUE_H
