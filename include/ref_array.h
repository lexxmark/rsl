#pragma once

#ifndef REF_ARRAY_H
#define REF_ARRAY_H

#include "ref_ptr.h"
#include "ref_iterator.h"
#include "ref_index.h"
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
	const auto& get_trackable(const array_ref<T, N>& array)
	{
		return array.get_trackable();
	}

} // end namespace rsl

#endif // REF_ARRAY_H
