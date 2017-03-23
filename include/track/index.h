#pragma once

#ifndef TRACK_INDEX_H
#define TRACK_INDEX_H

namespace rsl
{
	namespace track
	{
		template <class Container_t>
		auto cget_ptr(const Container_t& container, size_t index)
		{
			RSL_EXPECT(index < container.size());
			return make_ptr(&container[index], get_trackable(container));
		}

		template <class Container_t>
		auto get_ptr(Container_t& container, size_t index)
		{
			RSL_EXPECT(index < container.size());
			return make_ptr(&container[index], get_trackable(container));
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_INDEX_H
