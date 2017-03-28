#pragma once

#ifndef TRACK_PTR_H
#define TRACK_PTR_H

#include "pointer_policy.h"
#include <memory>

namespace rsl
{
	namespace track
	{
		struct trackable
		{
			trackable() = default;
			trackable(std::nullptr_t) {}

			~trackable()
			{
				reset();
			}

			trackable(const trackable&) {}
			trackable& operator=(const trackable&) { return *this; }

			trackable(trackable&&) {}
			trackable& operator=(trackable&&) { return *this; }

			void reset() const
			{
				if (m_ptr)
					m_ptr->on_dangle();
			}

			void reset_range(void* ptr_begin, void* ptr_end) const
			{
				if (m_ptr)
					m_ptr->on_dangle(ptr_begin, ptr_end);
			}

			struct pointer_base
			{
			protected:
				pointer_base() = default;

				pointer_base(const trackable& trackable_object)
				{
					connect(trackable_object.m_ptr);
				}

				pointer_base(const pointer_base& other)
				{
					connect(other);
				}

				pointer_base& operator=(const pointer_base& other)
				{
					disconnect();
					connect(other);
					return *this;
				}

				pointer_base(pointer_base&& other)
				{
					replace(std::move(other));
				}

				pointer_base& operator=(pointer_base&& other)
				{
					disconnect();
					replace(std::move(other));
					return *this;
				}

			protected:
				virtual void on_dangle_impl() = 0;
				virtual bool on_dangle_impl(void* ptr_begin, void* ptr_end) = 0;

			private:
				~pointer_base()
				{
					disconnect();
				}
				
				void connect(const pointer_base& other)
				{
					RSL_EXPECT(!(m_next || m_previous_next));
					connect(other.m_next);
				}

				void connect(pointer_base*& ptr_chain)
				{
					m_previous_next = &ptr_chain;
					m_next = ptr_chain;
					ptr_chain = this;
					if (m_next)
						m_next->m_previous_next = &m_next;
				}

				void replace(pointer_base&& other)
				{
					RSL_EXPECT(!(m_next || m_previous_next));
					if (other.m_previous_next)
					{
						std::swap(m_next, other.m_next);
						std::swap(m_previous_next, other.m_previous_next);
						*m_previous_next = this;
						if (m_next)
							m_next->m_previous_next = &m_next;
					}
				}

				void disconnect()
				{
					if (!m_previous_next)
						return;

					*m_previous_next = m_next;
					if (m_next)
						m_next->m_previous_next = m_previous_next;

					m_previous_next = nullptr;
					m_next = nullptr;
				}

				void on_dangle()
				{
					if (m_next)
						m_next->on_dangle();

					on_dangle_impl();
					disconnect();
				}

				bool on_dangle(void* ptr_begin, void* ptr_end)
				{
					RSL_EXPECT(ptr_begin < ptr_end);

					bool result = false;

					if (m_next)
						result = m_next->on_dangle(ptr_begin, ptr_end);

					if (on_dangle_impl(ptr_begin, ptr_end))
					{
						disconnect();
						result = true;
					}

					return result;
				}

				mutable pointer_base* m_next = nullptr;
				mutable pointer_base** m_previous_next = nullptr;

				friend struct trackable;
			};

			template <typename T, typename NullPtrPolicy, typename OnDanglePolicy>
			class pointer : public pointer_base
			{
			public:
				pointer() { on_assign(nullptr); }
				pointer(std::nullptr_t) { on_assign(nullptr); }

				pointer(T* ptr, const trackable& trackable_object)
					: pointer_base(trackable_object),
					m_ptr(ptr)
				{
					on_assign(m_ptr);
				}

				template <typename U, typename N, typename D>
				pointer(const pointer<U, N, D>& other)
					: pointer_base(other),
					m_ptr(static_cast<T*>(other.get()))
				{
					on_assign(m_ptr);
				}

				template <typename U, typename N, typename D>
				pointer(T* ptr, const pointer<U, N, D>& other)
					: pointer_base(other),
					m_ptr(ptr)
				{
					on_assign(m_ptr);
				}

				~pointer() = default;

				pointer(const pointer& other)
					: pointer_base(other),
					m_ptr(other.m_ptr)
				{
					on_assign(m_ptr);
				}
				pointer& operator=(const pointer& other)
				{
					pointer_base::operator=(other);
					m_ptr = other.m_ptr;
					on_assign(m_ptr);
					return *this;
				}

				pointer(pointer&& other)
					: pointer_base(std::move(other)),
					m_ptr(std::move(other.m_ptr))
				{
					on_assign(m_ptr);
				}
				pointer& operator=(pointer&& other)
				{
					pointer_base::operator=(std::move(other));
					m_ptr = std::move(other.m_ptr);
					on_assign(m_ptr);
					return *this;
				}

				pointer& operator=(std::nullptr_t)
				{
					*this = pointer<T>();
					on_assign(nullptr);
					return *this;
				}

				explicit operator bool() const
				{
					return get() != nullptr;
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
					return m_ptr;
				}

			private:
				void on_dangle_impl() final
				{
					OnDanglePolicy::on_dangle(m_ptr);
				}

				bool on_dangle_impl(void* ptr_begin, void* ptr_end) final
				{
					if (ptr_begin <= m_ptr && m_ptr < ptr_end)
					{
						OnDanglePolicy::on_dangle(m_ptr);
						return true;
					}
					return false;
				}

				template <typename U>
				void on_assign(U ptr)
				{
					NullPtrPolicy::on_assign(ptr);
				}

				T* m_ptr = nullptr;
			};

		private:
			mutable pointer_base* m_ptr = nullptr;
		};

		template <typename T, typename NullPtrPolicy = NULL_PTR_POLICY_DEFAULT(T), typename OnDanglePolicy = ON_DANGLE_POLICY_DEFAULT(T)>
		using pointer = trackable::pointer<T, NullPtrPolicy, OnDanglePolicy>;

		template <typename T>
		pointer<T> make_ptr(T* ptr, const trackable& trackable_object)
		{
			if (!ptr)
				return pointer<T>();

			return pointer<T>(ptr, trackable_object);
		}

		template <typename T>
		pointer<T> make_ptr(T* ptr)
		{
			return make_ptr<T>(ptr, *static_cast<const trackable*>(ptr));
		}

		template <typename T>
		pointer<T> make_ptr(std::nullptr_t)
		{
			return pointer<T>();
		}

		template <typename T, typename A>
		pointer<T> make_ptr(T* ptr, const pointer<A>& assoc_ptr)
		{
			return pointer<T>(ptr, assoc_ptr);
		}
	} // end namespace track
} // end namespace rsl

#endif // TRACK_PTR_H
