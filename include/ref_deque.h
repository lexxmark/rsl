#pragma once

#ifndef REF_DEQUE_H
#define REF_DEQUE_H

#include "ref_allocator.h"
#include <vector>

namespace rsl
{
	template <typename T, typename Alloc = std::allocator<T>>
	using deque_ref = std::deque<T, ref_allocator<T, Alloc>>;

	template <class T, class Alloc = std::allocator<T>>
	const auto& get_trackable(const deque_ref<T, Alloc>& deque)
	{
		return deque.get_allocator().get_trackable();
	}

} // end namespace rsl

#endif // REF_DEQUE_H
