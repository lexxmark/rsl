#pragma once

#ifndef TRACK_PTR_H
#define TRACK_PTR_H

#include "../rsl_def.h"
#include <stdexcept>
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
				{
					RSL_ON_DANGLING;
					m_ptr->disconnect_chain();
				}
			}

			void reset_range(void* ptr_begin, void* ptr_end) const
			{
				if (m_ptr && m_ptr->disconnect_chain(ptr_begin, ptr_end))
				{
					RSL_ON_DANGLING;
				}
			}

			struct pointer_base
			{
			protected:
				pointer_base() = default;

				pointer_base(void* ptr, const trackable& trackable_object)
					: m_ptr(ptr)
				{
					RSL_EXPECT(m_ptr);
					connect(trackable_object.m_ptr);
				}

				~pointer_base()
				{
					if (m_ptr)
						disconnect();
				}

				pointer_base(const pointer_base& other)
				{
					connect(other.get(), other);
				}

				pointer_base(void* ptr, const pointer_base& other)
				{
					connect(ptr, other);
				}

				pointer_base& operator=(const pointer_base& other)
				{
					if (m_ptr)
						disconnect();
					connect(other.get(), other);
					return *this;
				}

				pointer_base(pointer_base&& other)
				{
					replace(std::move(other));
				}

				pointer_base& operator=(pointer_base&& other)
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
				void connect(void* ptr, const pointer_base& other)
				{
					RSL_EXPECT(!(m_ptr || m_next || m_previous_next));
					if (other.m_ptr)
					{
						m_ptr = ptr;
						connect(other.m_next);
					}
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

				mutable pointer_base* m_next = nullptr;
				mutable pointer_base** m_previous_next = nullptr;

				friend struct trackable;
			};

			template <typename T>
			class pointer : public pointer_base
			{
			public:
				pointer() = default;
				pointer(std::nullptr_t) {}

				pointer(T* ptr, const trackable& trackable_object)
					: pointer_base(rsl::details::cast_to_void(ptr), trackable_object)
				{
				}

				template <typename U>
				pointer(const pointer<U>& other)
					: pointer_base(other)
				{
				}

				template <typename U>
				pointer(T* p, const pointer<U>& other)
					: pointer_base(details::cast_to_void(p), other)
				{
				}

				~pointer() = default;

				pointer(const pointer& other) = default;
				pointer& operator=(const pointer& other) = default;

				pointer(pointer&& other) = default;
				pointer& operator=(pointer&& other) = default;

				explicit operator bool() const
				{
					return pointer_base::get() != nullptr;
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
					return reinterpret_cast<T*>(pointer_base::get());
				}
			};

		private:
			mutable pointer_base* m_ptr = nullptr;
		};

		template <typename T>
		using pointer = trackable::pointer<T>;

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
