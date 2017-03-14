#pragma once

#ifndef REF_UNIQUE_PTR_H
#define REF_UNIQUE_PTR_H

#include "ref_ptr.h"
#include <memory>

namespace rsl
{
	namespace detail
	{
		template <class T, class D>
		class unique_ptr_ref_deleter : public D, public lifetime_trackable
		{
		public:
			static_assert(sizeof(lifetime_trackable) > 1, "lifetime_trackable should not be empty");

			constexpr unique_ptr_ref_deleter() noexcept = default;

			template<class... Types>
			unique_ptr_ref_deleter(Types&&... args)
				: D(std::forward<Types>(args)...)
			{
			}

			void operator()(T* ptr) const
			{
				lifetime_trackable::reset();
				D::operator()(ptr);
			}
		};

		template <class T, class D>
		class unique_ptr_ref_deleter<T[], D> : public D, public lifetime_trackable
		{
		public:
			static_assert(sizeof(lifetime_trackable) > 1, "lifetime_trackable should not be empty");

			constexpr unique_ptr_ref_deleter() noexcept = default;

			template<class... Types>
			unique_ptr_ref_deleter(Types&&... args)
				: D(std::forward<Types>(args)...)
			{
			}

			template<class U, class = typename std::enable_if_t<std::is_convertible_v<U(*)[], T(*)[]>, void>>
				void operator()(U* ptr) const
			{
				lifetime_trackable::reset();
				D::operator()(ptr);
			}
		};
	} // end namespace detail

	template<class T, class D = std::default_delete<T>>
	using unique_ptr_ref = std::unique_ptr<T, detail::unique_ptr_ref_deleter<T, D>>;

	template<class T, class... Types>
	inline std::enable_if_t<!std::is_array_v<T>, unique_ptr_ref<T>>
		make_unique_ref(Types&&... args)
	{
		auto ptr = std::make_unique<T>(std::forward<Types>(args)...);
		return unique_ptr_ref<T>(ptr.release());
	}

	template<class T, class... Types>
	inline std::enable_if_t<std::is_array_v<T> && std::extent<T>::value == 0, unique_ptr_ref<T>>
		make_unique_ref(size_t size)
	{
		auto ptr = std::make_unique<T>(size);
		return unique_ptr_ref<T>(ptr.release());
	}

	template <class T, class D = std::default_delete<T>>
    auto get_ref(const unique_ptr_ref<T, D>& ptr)
    {
		if (!ptr)
			return ref_ptr<std::decay_t<decltype(*ptr.get())>>();

		auto trackable = static_cast<const lifetime_trackable*>(&ptr.get_deleter());
		return make_ref(ptr.get(), *trackable);
    }

	template <class T, class D = std::default_delete<T>>
	auto get_ref(const unique_ptr_ref<T, D>& ptr, size_t index)
	{
		if (!ptr)
			return ref_ptr<std::decay_t<decltype(ptr[index])>>();

		auto trackable = static_cast<const lifetime_trackable*>(&ptr.get_deleter());
		return make_ref(&ptr[index], *trackable);
	}

} // end namespace rsl

#endif // REF_UNIQUE_PTR_H
