#pragma once

#ifndef REF_INDEX_H
#define REF_INDEX_H

namespace rsl
{
	template <class Container_t>
	auto get_cref(const Container_t& container, size_t index)
	{
		RSL_EXPECT(index < container.size());
		return make_ref(&container[index], get_trackable(container));
	}

	template <class Container_t>
	auto get_ref(Container_t& container, size_t index)
	{
		RSL_EXPECT(index < container.size());
		return make_ref(&container[index], get_trackable(container));
	}
} // end namespace rsl

#endif // REF_INDEX_H
