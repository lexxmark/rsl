#pragma once

#ifndef TRACK_DEQUE_H
#define TRACK_DEQUE_H

#include "ref_allocator.h"
#include <deque>

namespace rsl
{
	namespace track
	{
        template <typename T, typename Alloc = std::allocator<T>>
        using deque = std::deque<T, allocator<T, Alloc>>;

        template <class T, class Alloc = std::allocator<T>>
        const auto& get_trackable(const deque<T, Alloc>& d)
        {
            return d.get_allocator().get_trackable();
        }
	} // end namespace track
} // end namespace rsl

#endif // TRACK_DEQUE_H
