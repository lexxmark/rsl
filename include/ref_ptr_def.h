#pragma once

#ifndef REF_PTR_DEF_H
#define REF_PTR_DEF_H

#ifdef RSL_ASSERT_ON_DANGLING
    #include <assert.h>
    #define RSL_ON_DANGLING assert(false && "Some ref_ptr's are going to dangle.")
#endif
#ifdef RSL_THROW_ON_DANGLING
    #define RSL_ON_DANGLING throw ref_ptr_error("Some ref_ptr's are going to dangle.")
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
    #define RSL_EXPECT(cond) if (cond) { throw ref_ptr_error("Some ref_ptr's are going to dangle."); }
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

#endif // REF_PTR_DEF_H
