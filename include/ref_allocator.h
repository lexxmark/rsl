#pragma once

#ifndef REF_ALLOCATOR_H
#define REF_ALLOCATOR_H

#include "ref_ptr.h"
#include <memory>

namespace rsl
{
	template<class T, class Alloc = std::allocator<T>>
	class ref_allocator : public Alloc
	{
	public:
		typedef T value_type;

		typedef value_type *pointer;
		typedef const value_type *const_pointer;

		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef size_t size_type;
		typedef std::ptrdiff_t difference_type;

		//typedef Alloc::propagate_on_container_move_assignment propagate_on_container_move_assignment;
		typedef std::true_type is_always_equal;

		template<class Other>
		struct rebind
		{
			typedef ref_allocator<Other> other;
		};

		pointer address(reference val) const
		{
			return Alloc::address(val);
		}

		const_pointer address(const_reference val) const
		{
			return Alloc::address(val);
		}

		ref_allocator()
			: Alloc(),
			trackable_ref(make_ref(&trackable))
		{
		}

		ref_allocator(const ref_allocator& other)
			: Alloc(other),
			trackable_ref(make_ref(&other.get_trackable()))
		{
		}

		template<class Other>
		ref_allocator(const ref_allocator<Other>& other)
			: Alloc(other),
			trackable_ref(make_ref(&other.get_trackable()))
		{
		}

		template<class Other>
		ref_allocator<T>& operator=(const ref_allocator<Other>& other)
		{
			Alloc::operator=(other);
			trackable_ref = make_ref(&other.trackable);
			return (*this);
		}

		ref_allocator(ref_allocator&& other)
			: Alloc(other),
				trackable_ref(make_ref(&other.get_trackable()))
		{
		}

		ref_allocator& operator=(ref_allocator&& other)
		{
			Alloc::operator=(other);
			trackable_ref = make_ref(&other.trackable);
			return (*this);
		}

		~ref_allocator()
		{
		}

		void deallocate(pointer ptr, size_type count)
		{
			Alloc::deallocate(ptr, count);
		}

		pointer allocate(size_type count)
		{
			return Alloc::allocate(count);
		}

		pointer allocate(size_type count, const void* hint)
		{
			return Alloc::allocate(count);
		}

		template<class Objty, class... Types>
		void construct(Objty *ptr, Types&&... args)
		{
			Alloc::construct(ptr, std::forward<Types>(args)...);
		}

		template<class Uty>
		void destroy(Uty *ptr)
		{
			using namespace details;
			get_trackable().reset_range(cast_to_void(&ptr[0]), cast_to_void(&ptr[1]));
			Alloc::destroy(ptr);
		}

		size_t max_size() const
		{
			return Alloc::max_size();
		}

		const auto& get_trackable() const
		{
			if (trackable_ref)
				return *trackable_ref;
			else
				return trackable;
		}

	private:
		lifetime_trackable trackable;
		ref_ptr<lifetime_trackable> trackable_ref;
	};

} // end namespace rsl

#endif // REF_ALLOCATOR_H
