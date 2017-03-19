#pragma once

#ifndef REF_UNIQUE_PTR_H
#define REF_UNIQUE_PTR_H

#include "ref_ptr.h"
#include <array>

namespace rsl
{
	template<class T, std::size_t N>
	class array_ref : public std::array<T, N>
	{
	public:
		typedef std::array<T, N> array_type;

		using array_type::array;
		using array_type::operator[];
		using array_type::operator=;

		const auto& get_trackable() const { return trackable; }

	private:
		lifetime_trackable trackable;
	};

	template <class T, std::size_t N>
	auto get_ref(array_ref<T, N>& array, size_t index)
	{
		return make_ref(&array[index], array.get_trackable());
	}

	template <class T, std::size_t N>
	auto get_cref(const array_ref<T, N>& array, size_t index)
	{
		return make_ref(&array[index], array.get_trackable());
	}
} // end namespace rsl

#endif // REF_UNIQUE_PTR_H
