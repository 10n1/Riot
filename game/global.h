/*
 * global.h
 * graphicsDevice
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __graphicsDevice_global_h__
#define __graphicsDevice_global_h__

/* C headers */
#include <stdlib.h>
#ifdef _WIN32
    #include <crtdbg.h>
#endif
/* C++ headers */
/* External headers */
/* Internal headers */

/*******************************************************************\
External Constants And types
\*******************************************************************/
#define UNUSED_PARAMETER(param) (void)(sizeof((param)))
#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))
#define malloc_and_zero(pointer, type) pointer = (type)malloc(sizeof(*pointer)); memset(pointer, 0, sizeof(*pointer))
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif
#define SAFE_DELETE(p) if(p){ delete p; p = NULL; }

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

#ifndef NULL
    #if defined(__GNUG__)
        #define NULL __null
    #else
        #ifdef __cplusplus
            #define NULL 0
        #else
            #define NULL ((void*)0)
        #endif
    #endif
#endif

#if defined( _MSC_VER )
    #pragma warning(disable:4204) /* Non-constant aggregate initializer */
    #pragma warning(disable:4996) /* Depricated functions (*_s versions of CRT) */
#endif

#ifdef _WIN32
    /* Redefine malloc for windows */
    #ifndef malloc
        #define malloc(x) _malloc_dbg(x, _NORMAL_BLOCK, __FILE__, __LINE__)
    #endif
#endif

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
