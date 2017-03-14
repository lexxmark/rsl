#pragma once

#ifndef REF_ALLOCATOR_H
#define REF_ALLOCATOR_H

#include "ref_ptr.h"
#include <memory>

namespace rsl
{
	template<class T, class Alloc = std::allocator<T>>
	class ref_allocator
	{
	public:
		typedef T value_type;

		typedef value_type *pointer;
		typedef const value_type *const_pointer;

		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		//typedef Alloc::propagate_on_container_move_assignment propagate_on_container_move_assignment;
		typedef std::true_type is_always_equal;

		typedef char store_type;

		typedef typename Alloc::template rebind<store_type>::other store_alloc_t;
		typedef typename Alloc::template rebind<lifetime_trackable>::other lifetime_trackable_alloc_t;

		static auto get_storage_size(size_t count)
		{
			return sizeof(lifetime_trackable) + count * sizeof(T)
		}
		static auto get_storage(T* ptr)
		{
			auto p = (store_type*)ptr - sizeof(lifetime_trackable);
			return p;
		}

		static auto get_lifetime_trackable(T* ptr)
		{
			return (lifetime_trackable*)get_storage(ptr);
		}

		static auto get_object(store_type* ptr)
		{
			auto p = ptr + sizeof(lifetime_trackable);
			return (T*)p;
		}

		template<class Other>
		struct rebind
		{
			typedef allocator<typename Other> other;
		};

		pointer address(reference val) const
		{
			Alloc alloc;
			return alloc.address(val);
		}

		const_pointer address(const_reference val) const
		{
			Alloc alloc;
			return alloc.address(val);
		}

		ref_allocator() = default;
		ref_allocator(const ref_allocator&) = default;

		template<class Other>
		ref_allocator(const ref_allocator<Other>&)
		{}

		template<class Other>
		ref_allocator<T>& operator=(const ref_allocator<Other>&)
		{
			return (*this);
		}

		void deallocate(pointer ptr, size_type count)
		{
			auto p = get_storage(ptr);

			{
				lifetime_trackable_alloc_t alloc;
				alloc.destroy((lifetime_trackable*)p);
			}

			{
				store_alloc_t alloc;
				alloc.deallocate(p, get_storage_size(count));
			}
		}

		pointer allocate(size_type count)
		{
			store_type* ptr = nullptr;

			{
				store_alloc_t alloc;
				ptr = alloc.allocate(get_storage_size(count));
			}

			{
				lifetime_trackable_alloc_t alloc;
				alloc.construct((lifetime_trackable*)ptr);
			}

			return get_object(ptr);
		}

		pointer allocate(size_type count, const void* hint)
		{
			store_type* ptr = nullptr;

			{
				store_alloc_t alloc;
				ptr = alloc.allocate(get_storage_size(count), hint);
			}

			{
				lifetime_trackable_alloc_t alloc;
				alloc.construct((lifetime_trackable*)ptr);
			}

			return get_object(ptr);
		}

		template<class Objty,
			class... Types>
			void construct(Objty *ptr, Types&&... args)
		{
			Alloc alloc;
			alloc.construct(ptr, std::forward<Types>(args)...);
		}

		template<class Uty>
		void destroy(Uty *ptr)
		{
			Alloc alloc;
			alloc.destroy(ptr);
		}

		size_t max_size() const
		{
			Alloc alloc;
			return alloc.max_size();
		}
	};

} // end namespace rsl

#endif // REF_ALLOCATOR_H
