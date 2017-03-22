#pragma once

#ifndef REF_VECTOR_H
#define REF_VECTOR_H

#include "ref_allocator.h"
#include "ref_iterator.h"
#include "ref_index.h"
#include <vector>

namespace rsl
{
	template <typename T, typename Alloc = std::allocator<T>>
	using vector_ref = std::vector<T, ref_allocator<T, Alloc>>;

	template <class T, class Alloc = std::allocator<T>>
	const auto& get_trackable(const vector_ref<T, Alloc>& vector)
	{
		return vector.get_allocator().get_trackable();
	}

} // end namespace rsl

#endif // REF_VECTOR_H
