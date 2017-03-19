#pragma once

#ifndef REF_VECTOR_H
#define REF_VECTOR_H

#include "ref_allocator.h"
#include <vector>

namespace rsl
{
	template <typename T, typename Alloc = std::allocator<T>>
	using vector_ref = std::vector<T, ref_allocator<T, Alloc>>;

	template <class T, class Alloc = std::allocator<T>>
	auto get_cref(const vector_ref<T, Alloc>& vector, size_t index)
	{
		auto alloc = vector.get_allocator();
		return make_ref(&vector[index], alloc.get_trackable());
	}

	template <class T, class Alloc = std::allocator<T>>
	auto get_ref(vector_ref<T, Alloc>& vector, size_t index)
	{
		auto alloc = vector.get_allocator();
		return make_ref(&vector[index], alloc.get_trackable());
	}
} // end namespace rsl

#endif // REF_VECTOR_H
