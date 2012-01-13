/*
 *  renderEngineInternal.h
 *  renderEngine.lib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __renderEngineInternalLib_h__
#define __renderEngineInternalLib_h__

//#include <cheaders>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"


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

namespace Render
{

/*******************************************************************\
 External constants and types
\*******************************************************************/

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
} // namespace Render

#endif /* include guard */
