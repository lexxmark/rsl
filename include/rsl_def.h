#pragma once

#ifndef REF_PTR_DEF_H
#define REF_PTR_DEF_H

#include <stdexcept>

#ifdef RSL_ASSERT_ON_DANGLING
    #include <assert.h>
    #define RSL_ON_DANGLING assert(false && "Some ref_ptr's are going to dangle.")
#endif
#ifdef RSL_THROW_ON_DANGLING
    #define RSL_ON_DANGLING throw rsl::rsl_error("Some ref_ptr's are going to dangle.")
#endif
#ifdef RSL_TERMINATE_ON_DANGLING
    #define RSL_ON_DANGLING std::terminate()
#endif
#ifdef RSL_CUSTOM_ACTION_ON_DANGLING
    #define RSL_ON_DANGLING { RSL_CUSTOM_ACTION_ON_DANGLING; }
#endif
#ifndef RSL_ON_DANGLING
    #define RSL_ON_DANGLING
#endif

#ifdef RSL_ASSERT_ON_EXPECT
    #ifndef RSL_ASSERT_ON_DANGLING
        #include <assert.h>
    #endif
    #define RSL_EXPECT(cond) assert(cond)
#endif
#ifdef RSL_THROW_ON_EXPECT
    #define RSL_EXPECT(cond) if (cond) { throw rsl::rsl_error("Reached unexpected condition."); }
#endif
#ifdef RSL_TERMINATE_ON_EXPECT
    #define RSL_EXPECT(cond) if (cond) { std::terminate(); }
#endif
#ifdef RSL_CUSTOM_ACTION_ON_EXPECT
    #define RSL_EXPECT(cond) if (cond) { RSL_CUSTOM_ACTION_ON_EXPECT; }
#endif
#ifndef RSL_EXPECT
    #define RSL_EXPECT(cond)
#endif

namespace rsl
{
	struct rsl_error : public std::runtime_error
	{
		explicit rsl_error(char const* const message)
			: std::runtime_error(message)
		{}
	};

	namespace details
	{
		template <class T>
		inline void* cast_to_void(T* ptr)
		{
			return const_cast<void*>(reinterpret_cast<const void*>(ptr));
		}
	} // end namespace details
} // end namespace rsl

#endif // REF_PTR_DEF_H
