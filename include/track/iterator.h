#pragma once

#ifndef TRACK_ITERATOR_H
#define TRACK_ITERATOR_H

#include "pointer.h"

namespace rsl
{
	namespace track
	{
		template <class Container_t>
		auto cget_ptr(const Container_t& container, typename Container_t::const_iterator it)
		{
			RSL_EXPECT(it != std::end(container));
			return make_ptr(&(*it), get_trackable(container));
		}

		template <class Container_t>
		auto get_ptr(Container_t& container, typename Container_t::iterator it)
		{
			RSL_EXPECT(it != std::end(container));
			return make_ptr(&(*it), get_trackable(container));
		}

		template <class Container_t>
		auto cfind_ptr(const Container_t& container, const typename Container_t::key_type& key)
		{
			auto it = container.find(key);
			RSL_EXPECT(it != std::end(container));
			return make_ptr(&(*it), get_trackable(container));
		}

		template <class Container_t>
		auto find_ptr(Container_t& container, const typename Container_t::key_type& key)
		{
			auto it = container.find(key);
			RSL_EXPECT(it != std::end(container));
			return make_ptr(&(*it), get_trackable(container));
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_ITERATOR_H
