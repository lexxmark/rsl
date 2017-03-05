#pragma once

#ifndef REF_PTR_H
#define REF_PTR_H
#include <assert.h>

namespace rsl
{

#ifdef RSL_ENABLE_CHECKS
#define RSL_EXPECT(cond) assert(cond)
#else
#define RSL_EXPECT(cond)
#endif

    struct lifetime_trackable
    {
        lifetime_trackable() = default;

        ~lifetime_trackable()
        {
            if (m_ref_ptr)
                m_ref_ptr->disconnect_chain();
        }

        lifetime_trackable(const lifetime_trackable&) {}
        lifetime_trackable& operator=(const lifetime_trackable&) {}

        lifetime_trackable(lifetime_trackable&&) {}
        lifetime_trackable& operator=(lifetime_trackable&&) {}

        struct ref_ptr_base
        {
        protected:
            ref_ptr_base() = default;

            ref_ptr_base(lifetime_trackable& trackable, void* ptr)
                : m_ptr(ptr)
            {
                RSL_EXPECT(m_ptr);
                connect(trackable.m_ref_ptr);
            }

            ~ref_ptr_base()
            {
                disconnect();
            }

            ref_ptr_base(const ref_ptr_base& other)
            {
                connect(other);
            }

            ref_ptr_base& operator=(const ref_ptr_base& other)
            {
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
                disconnect();
                replace(std::move(other));
                return *this;
            }

            void* get() const
            {
                RSL_EXPECT(m_ptr);
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
                    *m_next->m_previous_next = *m_previous_next;
                m_ptr = nullptr;
            }

            void disconnect_chain()
            {
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
            ref_ptr() = default;
            ref_ptr(nullptr_t) {}

            ref_ptr(lifetime_trackable& trackable, T* ptr)
                : ref_ptr_base(trackable, ptr)
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

            operator bool() const
            {
                return ref_ptr_base::get() != nullptr;
            }

            T* get() const
            {
                return (T*)ref_ptr_base::get();
            }
        };

    private:
        ref_ptr_base* m_ref_ptr = nullptr;
    };

    template <typename T>
    using ref_ptr = lifetime_trackable::ref_ptr<T>;

    template <typename T>
    auto make_ref(lifetime_trackable& trackable, T* ptr)
    {
        return ref_ptr<T>(trackable, ptr);
    }

    template <typename T>
    auto make_ref(T* ptr)
    {
        return ref_ptr<T>(*ptr, ptr);
    }

} // end namespace rsl

#endif // REF_PTR_H
