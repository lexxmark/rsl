#pragma once

#ifndef REF_PTR_H
#define REF_PTR_H

#include "ref_ptr_def.h"
#include <stdexcept>

namespace rsl
{
    struct ref_ptr_error : public std::runtime_error
    {
        explicit ref_ptr_error(char const* const message)
            : std::runtime_error(message)
        {}
    };

    struct lifetime_trackable
    {
		constexpr lifetime_trackable() noexcept = default;
		constexpr lifetime_trackable(nullptr_t) noexcept {}

        ~lifetime_trackable()
        {
			reset();
        }

		lifetime_trackable(const lifetime_trackable&) noexcept {}
		lifetime_trackable& operator=(const lifetime_trackable&) noexcept { return *this; }

		lifetime_trackable(lifetime_trackable&&) noexcept {}
		lifetime_trackable& operator=(lifetime_trackable&&) noexcept { return *this; }

		void reset() const
		{
			if (m_ref_ptr)
			{
				RSL_ON_DANGLING;
				m_ref_ptr->disconnect_chain();
			}
		}

        struct ref_ptr_base
        {
        protected:
            constexpr ref_ptr_base() noexcept = default;

            ref_ptr_base(void* ptr, const lifetime_trackable& trackable)
                : m_ptr(ptr)
            {
                RSL_EXPECT(m_ptr);
                connect(trackable.m_ref_ptr);
            }

            ~ref_ptr_base()
            {
				if (m_ptr)
					disconnect();
            }

            ref_ptr_base(const ref_ptr_base& other)
            {
                connect(other);
            }

            ref_ptr_base& operator=(const ref_ptr_base& other)
            {
				if (m_ptr)
					disconnect();
                connect(other);
                return *this;
            }

            ref_ptr_base(ref_ptr_base&& other)
            {
                replace(std::move(other));
            }

            ref_ptr_base& operator=(ref_ptr_base&& other)
            {
				if (m_ptr)
					disconnect();
                replace(std::move(other));
                return *this;
            }

            void* get() const
            {
                return m_ptr;
            }

        private:
            void connect(const ref_ptr_base& other)
            {
                RSL_EXPECT(!(m_ptr || m_next || m_previous_next));
                if (other.m_ptr)
                {
                    m_ptr = other.m_ptr;
                    connect(other.m_next);
                }
            }

            void connect(ref_ptr_base*& ptr_chain)
            {
                m_previous_next = &ptr_chain;
                m_next = ptr_chain;
                ptr_chain = this;
				if (m_next)
					m_next->m_previous_next = &m_next;
            }

            void replace(ref_ptr_base&& other)
            {
                RSL_EXPECT(!(m_ptr || m_next || m_previous_next));
                if (other.m_ptr)
                {
                    std::swap(m_ptr, other.m_ptr);
                    std::swap(m_next, other.m_next);
                    std::swap(m_previous_next, other.m_previous_next);
                    *m_previous_next = this;
                }
            }

            void disconnect()
            {
                *m_previous_next = m_next;
                if (m_next)
                    m_next->m_previous_next = m_previous_next;
                m_ptr = nullptr;
				m_previous_next = nullptr;
				m_next = nullptr;
            }

            void disconnect_chain()
            {
				RSL_EXPECT(m_ptr);

				if (m_next)
                    m_next->disconnect_chain();
                disconnect();
            }

            void* m_ptr = nullptr;

            mutable ref_ptr_base* m_next = nullptr;
            mutable ref_ptr_base** m_previous_next = nullptr;

            friend struct lifetime_trackable;
        };

        template <typename T>
        class ref_ptr : public ref_ptr_base
        {
        public:
			constexpr ref_ptr() noexcept = default;
			constexpr ref_ptr(nullptr_t) noexcept {}

            ref_ptr(T* ptr, const lifetime_trackable& trackable)
                : ref_ptr_base(ptr, trackable)
            {
            }

            template <typename U>
            ref_ptr(ref_ptr<U>& other)
                : ref_ptr_base(other)
            {
            }

            ~ref_ptr() = default;

            ref_ptr(const ref_ptr& other) = default;
            ref_ptr& operator=(const ref_ptr& other) = default;

            ref_ptr(ref_ptr&& other) = default;
            ref_ptr& operator=(ref_ptr&& other) = default;

            explicit operator bool() const noexcept
            {
                return ref_ptr_base::get() != nullptr;
            }

			typename std::add_lvalue_reference<T>::type operator*() const noexcept
			{
				RSL_EXPECT(get());
				return (*get());
			}

			T* operator->() const noexcept
			{
				return (std::pointer_traits<T*>::pointer_to(**this));
			}

            T* get() const
            {
                return (T*)ref_ptr_base::get();
            }
        };

    private:
        mutable ref_ptr_base* m_ref_ptr = nullptr;
    };

    template <typename T>
    using ref_ptr = lifetime_trackable::ref_ptr<T>;

    template <typename T>
    auto make_ref(T* ptr, const lifetime_trackable& trackable)
    {
		if (!ptr)
			return ref_ptr<T>();

        return ref_ptr<T>(ptr, trackable);
    }

    template <typename T>
    auto make_ref(T* ptr)
    {
        return make_ref<T>(ptr, *ptr);
    }

	template <typename T>
	auto make_ref(nullptr_t)
	{
		return ref_ptr<T>();
	}

} // end namespace rsl

#undef RSL_ON_DANGLING
#undef RSL_EXPECT

#endif // REF_PTR_H
