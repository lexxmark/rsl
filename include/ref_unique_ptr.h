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
		class unique_ptr_ref_deleter : public D
		{
		public:
			constexpr unique_ptr_ref_deleter() noexcept = default;

			template<class... Types>
			unique_ptr_ref_deleter(Types&&... args)
				: D(std::forward<Types>(args)...)
			{
			}

			void operator()(T* ptr) const
			{
				trackable.reset();
				D::operator()(ptr);
			}

			const auto& get_trackable() const { return trackable; }

		private:
			lifetime_trackable trackable;
		};

		template <class T, class D>
		class unique_ptr_ref_deleter<T[], D> : public D
		{
		public:
			constexpr unique_ptr_ref_deleter() noexcept = default;

			template<class... Types>
			unique_ptr_ref_deleter(Types&&... args)
				: D(std::forward<Types>(args)...)
			{
			}

			template<class U, class = typename std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value, void>>
				void operator()(U* ptr) const
			{
				trackable.reset();
				D::operator()(ptr);
			}

			const auto& get_trackable() const { return trackable; }

		private:
			lifetime_trackable trackable;
		};
	} // end namespace detail

	template<class T, class D = std::default_delete<T>>
	using unique_ptr_ref = std::unique_ptr<T, detail::unique_ptr_ref_deleter<T, D>>;

	template<class T, class... Types>
	inline std::enable_if_t<!std::is_array<T>::value, unique_ptr_ref<T>>
	make_unique_ref(Types&&... args)
	{
		auto ptr = std::make_unique<T>(std::forward<Types>(args)...);
		return unique_ptr_ref<T>(ptr.release());
	}

	template<class T, class... Types>
	inline std::enable_if_t<std::is_array<T>::value && std::extent<T>::value == 0, unique_ptr_ref<T>>
	make_unique_ref(size_t size)
	{
		auto ptr = std::make_unique<T>(size);
		return unique_ptr_ref<T>(ptr.release());
	}

	template <class T, class D = std::default_delete<T>>
    auto get_cref(const unique_ptr_ref<T, D>& ptr)
    {
		typedef const std::decay_t<decltype(*ptr.get())> value_t;

		if (!ptr)
			return ref_ptr<value_t>();

		const auto& trackable = ptr.get_deleter().get_trackable();
		return make_ref<value_t>(ptr.get(), trackable);
    }

	template <class T, class D = std::default_delete<T>>
	auto get_cref(const unique_ptr_ref<T, D>& ptr, size_t index)
	{
		typedef const std::decay_t<decltype(*ptr.get())> value_t;
	
		if (!ptr)
			return ref_ptr<value_t>();

		const auto& trackable = ptr.get_deleter().get_trackable();
		return make_ref<value_t>(&ptr[index], trackable);
	}

	template <class T, class D = std::default_delete<T>>
	auto get_ref(const unique_ptr_ref<T, D>& ptr)
	{
		typedef std::decay_t<decltype(*ptr.get())> value_t;

		if (!ptr)
			return ref_ptr<value_t>();

		const auto& trackable = ptr.get_deleter().get_trackable();
		return make_ref<value_t>(ptr.get(), trackable);
	}

	template <class T, class D = std::default_delete<T>>
	auto get_ref(const unique_ptr_ref<T, D>& ptr, size_t index)
	{
		typedef std::decay_t<decltype(*ptr.get())> value_t;

		if (!ptr)
			return ref_ptr<value_t>();

		const auto& trackable = ptr.get_deleter().get_trackable();
		return make_ref<value_t>(&ptr[index], trackable);
	}
} // end namespace rsl

#endif // REF_UNIQUE_PTR_H
