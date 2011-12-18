/*
 *  types.h
 *  engine
 *
 *  Created by Kyle Weicht on 12/17/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef riot_types_h_
#define riot_types_h_

/*******************************************************************\
 External constants and types
\*******************************************************************/
#if defined( WIN32 ) || defined( WIN64 )
#include <stdint.h>
#include <limits.h>
typedef unsigned __int8  byte;
typedef unsigned __int8  uint8;
typedef signed   __int8  sint8;
typedef signed   __int8  int8;
typedef unsigned __int16 uint16;
typedef signed   __int16 sint16;
typedef signed   __int16 int16;
typedef unsigned __int32 bool32;
typedef unsigned __int32 uint32;
typedef signed   __int32 sint32;
typedef signed   __int32 int32;
typedef unsigned __int64 uint64;
typedef signed   __int64 sint64;
typedef signed   __int64 int64;
#else // not defined( WIN32 ) || defined( WIN64 )
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
typedef uint8_t     byte;
typedef uint8_t     uint8;
typedef int8_t      sint8;
typedef int8_t      int8;
typedef uint16_t    uint16;
typedef int16_t     sint16;
typedef int16_t     int16;
typedef uint32_t    bool32;
typedef uint32_t    uint32;
typedef int32_t     sint32;
typedef int32_t     int32;
typedef uint64_t    uint64;
typedef int64_t     sint64;
typedef int64_t     int64;
#endif // defined( WIN32 ) || defined( WIN64 )

typedef uint32      uint;
typedef sint32      sint;
typedef float       float32;
typedef double      float64;

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

#endif /* include guard */
