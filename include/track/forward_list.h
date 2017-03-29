#pragma once

#ifndef TRACK_FORWARD_LIST_H
#define TRACK_FORWARD_LIST_H

#include "allocator.h"
#include "iterator.h"
#include "index.h"
#include <forward_list>

namespace rsl
{
	namespace track
	{
		template <typename T, typename Alloc = std::allocator<T>>
		using forward_list = std::forward_list<T, allocator<T, Alloc>>;

		template <class T, class Alloc = std::allocator<T>>
		const auto& get_trackable(const forward_list<T, Alloc>& lst)
		{
			return lst.get_allocator().get_trackable();
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_FORWARD_LIST_H
