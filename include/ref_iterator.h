#pragma once

#ifndef REF_ITERATOR_H
#define REF_ITERATOR_H

#include "ref_allocator.h"

namespace rsl
{
	template <class Container_t>
	auto get_cref(const Container_t& container, typename Container_t::const_iterator it)
	{
		RSL_EXPECT(it != std::cend(container));
		return make_ref(&(*it), get_trackable(container));
	}

	template <class Container_t>
	auto get_ref(Container_t& container, typename Container_t::iterator it)
	{
		RSL_EXPECT(it != std::end(container));
		return make_ref(&(*it), get_trackable(container));
	}

	template <class Container_t>
	auto find_cref(const Container_t& container, const typename Container_t::key_type& key)
	{
		auto it = container.find(key);
		RSL_EXPECT(it != std::cend(container));
		return make_ref(&(*it), get_trackable(container));
	}

	template <class Container_t>
	auto find_ref(Container_t& container, const typename Container_t::key_type& key)
	{
		auto it = container.find(key);
		RSL_EXPECT(it != std::end(container));
		return make_ref(&(*it), get_trackable(container));
	}

} // end namespace rsl

#endif // REF_ITERATOR_H
