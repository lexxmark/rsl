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
			static void on_assign(T* ptr) { RSL_EXPECT(ptr != nullptr); }
			static void on_assign(std::nullptr_t) { static_assert(false, "nullptr is forbidden"); }
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
		struct throw_on_dangle
		{
			static void on_dangle(T*) { throw rsl::rsl_error("pointer is going to dangle"); }
		};

#define NULL_PTR_POLICY_DEFAULT(T) may_be_null<T>
#define ON_DANGLE_POLICY_DEFAULT(T) null_on_dangle<T>
	} // end namespace track
} // end namespace rsl

#endif // TRACK_PTR_POLICY_H
