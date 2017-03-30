#pragma once

#ifndef TRACK_VECTOR_H
#define TRACK_VECTOR_H

#include "allocator.h"
#include "iterator.h"
#include "index.h"
#include <vector>

namespace rsl
{
	namespace track
	{
		template <typename T, typename Alloc = std::allocator<T>>
		using vector = std::vector<T, allocator<T, Alloc>>;

		template <class T, class Alloc = std::allocator<T>>
		const trackable& get_trackable(const vector<T, Alloc>& vec)
		{
			return vec.get_allocator().get_trackable();
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_VECTOR_H
