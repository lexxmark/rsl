#pragma once

#ifndef TRACK_LIST_H
#define TRACK_LIST_H

#include "allocator.h"
#include "iterator.h"
#include "index.h"
#include <list>

namespace rsl
{
	namespace track
	{
		template <typename T, typename Alloc = std::allocator<T>>
		using list = std::list<T, allocator<T, Alloc>>;

		template <class T, class Alloc = std::allocator<T>>
		const trackable& get_trackable(const list<T, Alloc>& lst)
		{
			return lst.get_allocator().get_trackable();
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_LIST_H
