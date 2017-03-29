#pragma once

#ifndef TRACK_PTR_POLICY_H
#define TRACK_PTR_POLICY_H

#include "../rsl_def.h"

namespace rsl
{
	namespace track
	{
		template <typename T>
		struct may_be_null
		{
			static void on_assign(T*) {}
		};
		template <typename T>
		struct not_null
		{
			template <typename U>
			static void on_assign(U ptr)
			{
				static_assert(!std::is_same<U, std::nullptr_t>::value, "nullptr is forbidden");
				RSL_EXPECT(ptr != nullptr);
			}
		};

		template <typename T>
		struct null_on_dangle
		{
			static void on_dangle(T*& ptr) { ptr = nullptr; }
		};
		template <typename T>
		struct assert_on_dangle
		{
			static void on_dangle(T*) { assert(false && "pointer is going to dangle"); }
		};
		template <typename T>
		struct terminate_on_dangle
		{
			static void on_dangle(T*) { std::terminate(); }
		};

	} // end namespace track
} // end namespace rsl

#ifndef RSL_NULL_PTR_POLICY_DEFAULT
#define RSL_NULL_PTR_POLICY_DEFAULT(T) rsl::track::may_be_null<T>
#endif

#ifndef RSL_ON_DANGLE_POLICY_DEFAULT
#define RSL_ON_DANGLE_POLICY_DEFAULT(T) rsl::track::null_on_dangle<T>
#endif

#endif // TRACK_PTR_POLICY_H
