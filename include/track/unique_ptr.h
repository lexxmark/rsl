#pragma once

#ifndef TRACK_UNIQUE_PTR_H
#define TRACK_UNIQUE_PTR_H

#include "pointer.h"

namespace rsl
{
	namespace track
	{
		namespace details
		{
			template <class T, class D>
			class deleter : public D
			{
			public:
				constexpr deleter() noexcept = default;

				template<class... Types>
				deleter(Types&&... args)
					: D(std::forward<Types>(args)...)
				{
				}

				void operator()(T* ptr) const
				{
					m_trackable.reset();
					D::operator()(ptr);
				}

				const auto& get_trackable() const { return m_trackable; }

			private:
				trackable m_trackable;
			};

			template <class T, class D>
			class deleter<T[], D> : public D
			{
			public:
				constexpr deleter() noexcept = default;

				template<class... Types>
				deleter(Types&&... args)
					: D(std::forward<Types>(args)...)
				{
				}

				template<class U, class = typename std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value, void>>
				void operator()(U* ptr) const
				{
					m_trackable.reset();
					D::operator()(ptr);
				}

				const auto& get_trackable() const { return m_trackable; }

			private:
				trackable m_trackable;
			};
		} // end namespace details

		template<class T, class D = std::default_delete<T>>
		using unique_ptr = std::unique_ptr<T, details::deleter<T, D>>;

		template<class T, class... Types>
		inline std::enable_if_t<!std::is_array<T>::value, unique_ptr<T>>
		make_unique(Types&&... args)
		{
			auto ptr = std::make_unique<T>(std::forward<Types>(args)...);
			return unique_ptr<T>(ptr.release());
		}

		template<class T, class... Types>
		inline std::enable_if_t<std::is_array<T>::value && std::extent<T>::value == 0, unique_ptr<T>>
		make_unique(size_t size)
		{
			auto ptr = std::make_unique<T>(size);
			return unique_ptr<T>(ptr.release());
		}

		template <class T, class D = std::default_delete<T>>
		auto cget_ptr(const unique_ptr<T, D>& ptr)
		{
			typedef const std::decay_t<decltype(*ptr.get())> value_t;

			if (!ptr)
				return pointer<value_t>();

			return make_ptr<value_t>(ptr.get(), ptr.get_deleter().get_trackable());
		}

		template <class T, class D = std::default_delete<T>>
		auto cget_ptr(const unique_ptr<T, D>& ptr, size_t index)
		{
			typedef const std::decay_t<decltype(*ptr.get())> value_t;

			if (!ptr)
				return pointer<value_t>();

			return make_ptr<value_t>(&ptr[index], ptr.get_deleter().get_trackable());
		}

		template <class T, class D = std::default_delete<T>>
		auto get_ptr(const unique_ptr<T, D>& ptr)
		{
			typedef std::decay_t<decltype(*ptr.get())> value_t;

			if (!ptr)
				return pointer<value_t>();

			return make_ptr<value_t>(ptr.get(), ptr.get_deleter().get_trackable());
		}

		template <class T, class D = std::default_delete<T>>
		auto get_ptr(const unique_ptr<T, D>& ptr, size_t index)
		{
			typedef std::decay_t<decltype(*ptr.get())> value_t;

			if (!ptr)
				return pointer<value_t>();

			return make_ptr<value_t>(&ptr[index], ptr.get_deleter().get_trackable());
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_UNIQUE_PTR_H
