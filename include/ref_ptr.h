#pragma once

#ifndef REF_PTR_H
#define REF_PTR_H

#include "ref_ptr_def.h"
#include <stdexcept>
#include <memory>

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
		lifetime_trackable() = default;
		lifetime_trackable(std::nullptr_t) {}

        ~lifetime_trackable()
        {
			reset();
        }

		lifetime_trackable(const lifetime_trackable&) {}
		lifetime_trackable& operator=(const lifetime_trackable&) { return *this; }

		lifetime_trackable(lifetime_trackable&&) {}
		lifetime_trackable& operator=(lifetime_trackable&&) { return *this; }

		void reset() const
		{
			if (m_ref_ptr)
			{
				RSL_ON_DANGLING;
				m_ref_ptr->disconnect_chain();
			}
		}

		void reset_range(void* ptr_begin, void* ptr_end) const
		{
			if (m_ref_ptr && m_ref_ptr->disconnect_chain(ptr_begin, ptr_end))
			{
				RSL_ON_DANGLING;
			}
		}

        struct ref_ptr_base
        {
        protected:
            ref_ptr_base() = default;

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
                connect(other.get(), other);
            }

			ref_ptr_base(void* ptr, const ref_ptr_base& other)
			{
				connect(ptr, other);
			}
			
			ref_ptr_base& operator=(const ref_ptr_base& other)
            {
				if (m_ptr)
					disconnect();
                connect(other.get(), other);
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

		protected:
            void* get() const
            {
                return m_ptr;
            }

        private:
            void connect(void* ptr, const ref_ptr_base& other)
            {
                RSL_EXPECT(!(m_ptr || m_next || m_previous_next));
                if (other.m_ptr)
                {
                    m_ptr = ptr;
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
					if (m_next)
						m_next->m_previous_next = &m_next;
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

			bool disconnect_chain(void* ptr_begin, void* ptr_end)
			{
				RSL_EXPECT(m_ptr);

				bool result = false;

				if (m_next)
					result = m_next->disconnect_chain(ptr_begin, ptr_end);

				if (ptr_begin <= m_ptr && m_ptr < ptr_end)
				{
					disconnect();
					result = true;
				}

				return result;
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
		ref_ptr() = default;
		ref_ptr(std::nullptr_t) {}

            ref_ptr(T* ptr, const lifetime_trackable& trackable)
                : ref_ptr_base((void*)ptr, trackable)
            {
            }

            template <typename U>
            ref_ptr(const ref_ptr<U>& other)
                : ref_ptr_base(other)
            {
            }

			template <typename U>
			ref_ptr(T* ptr, const ref_ptr<U>& other)
				: ref_ptr_base((void*)ptr, other)
			{
			}
			
			~ref_ptr() = default;

            ref_ptr(const ref_ptr& other) = default;
            ref_ptr& operator=(const ref_ptr& other) = default;

            ref_ptr(ref_ptr&& other) = default;
            ref_ptr& operator=(ref_ptr&& other) = default;

            explicit operator bool() const
            {
                return ref_ptr_base::get() != nullptr;
            }

			typename std::add_lvalue_reference<T>::type operator*() const
			{
				RSL_EXPECT(get());
				return (*get());
			}

			T* operator->() const
			{
				RSL_EXPECT(get());
				return get();
			}

            T* get() const
            {
                return reinterpret_cast<T*>(ref_ptr_base::get());
            }
        };

    private:
        mutable ref_ptr_base* m_ref_ptr = nullptr;
    };

    template <typename T>
    using ref_ptr = lifetime_trackable::ref_ptr<T>;

    template <typename T>
    ref_ptr<T> make_ref(T* ptr, const lifetime_trackable& trackable)
    {
		if (!ptr)
			return ref_ptr<T>();

        return ref_ptr<T>(ptr, trackable);
    }

    template <typename T>
    ref_ptr<T> make_ref(T* ptr)
    {
        return make_ref<T>(ptr, *static_cast<const lifetime_trackable*>(ptr));
    }

	template <typename T>
	ref_ptr<T> make_ref(std::nullptr_t)
	{
		return ref_ptr<T>();
	}

	template <typename T, typename A>
	ref_ptr<T> make_ref(T* ptr, const ref_ptr<A>& assoc_ptr)
	{
		return ref_ptr<T>(ptr, assoc_ptr);
	}

} // end namespace rsl

#undef RSL_ON_DANGLING
//#undef RSL_EXPECT

#endif // REF_PTR_H
