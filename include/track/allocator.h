#pragma once

#ifndef TRACK_ALLOCATOR_H
#define TRACK_ALLOCATOR_H

#include "pointer.h"

namespace rsl
{
	namespace track
	{
		template<class T, class Alloc = std::allocator<T>>
		class allocator : public Alloc
		{
		public:
			typedef T value_type;

			typedef value_type *pointer;
			typedef const value_type *const_pointer;

			typedef value_type& reference;
			typedef const value_type& const_reference;

			typedef size_t size_type;
			typedef std::ptrdiff_t difference_type;

			typedef std::true_type is_always_equal;

			template<class Other>
			struct rebind
			{
				typedef allocator<Other> other;
			};

			pointer address(reference val) const
			{
				return Alloc::address(val);
			}

			const_pointer address(const_reference val) const
			{
				return Alloc::address(val);
			}

			allocator()
				: Alloc(),
				m_trackable_ptr(make_ptr(&m_trackable))
			{
			}

			allocator(const allocator& other)
				: Alloc(other),
				m_trackable_ptr(make_ptr(&other.get_trackable()))
			{
			}

			template<class Other>
			allocator(const allocator<Other>& other)
				: Alloc(other),
				m_trackable_ptr(make_ptr(&other.get_trackable()))
			{
			}

			template<class Other>
			allocator<T>& operator=(const allocator<Other>& other)
			{
				Alloc::operator=(other);
				m_trackable_ptr = make_ptr(&other.get_trackable());
				return (*this);
			}

			allocator(allocator&& other)
				: Alloc(other),
				m_trackable_ptr(make_ptr(&other.get_trackable()))
			{
			}

			allocator& operator=(allocator&& other)
			{
				Alloc::operator=(other);
				m_trackable_ptr = make_ptr(&other.get_trackable());
				return (*this);
			}

			~allocator()
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
				if (m_trackable_ptr)
					return *m_trackable_ptr;
				else
					return m_trackable;
			}

		private:
			trackable m_trackable;
			track::pointer<trackable> m_trackable_ptr;
		};
	} // end namespace track
} // end namespace rsl

#endif // TRACK_ALLOCATOR_H
