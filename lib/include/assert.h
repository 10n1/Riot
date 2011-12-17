/*
 *  assert.h
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef _assert_h_
#define _assert_h_

/*
 * Define debugBreak
 */
#if defined( _MSC_VER )
    #define debugBreak() __debugbreak()
#elif defined( __GNUC__ )
    #define debugBreak() __asm__( "int $3\n" : : )
#else
    #error Unsupported compiler
#endif

/*
 * Define assert
 */
#ifndef assert
    #define assert(condition)   \
        do                      \
        {                       \
            if(!(condition))    \
            {                   \
                debugBreak();   \
            }                   \
        } while(__LINE__ == -1)
        /* This causes warning 4127 with VC++ (conditional expression is constant) */
    #if defined( _MSC_VER )
        #pragma warning(disable:4127)
    #endif /* defined( _MSC_VER ) */
#endif

#ifndef ASSERT
    #define ASSERT(condition) assert((condition))
#endif


#endif /* include guard */
