//
//  types.h
//  Cosmos
//
//  Created by Kyle Weicht on 7/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

/*
    Types
*/
#if defined( WIN32 ) || defined( WIN64 )
#include <limits.h>
typedef unsigned __int8  byte;
typedef unsigned __int8  uint8;
typedef signed   __int8  sint8;
typedef signed   __int8  int8;
typedef unsigned __int16 uint16;
typedef signed   __int16 sint16;
typedef signed   __int16 int16;
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
typedef uint64_t    int64;
#endif // defined( WIN32 ) || defined( WIN64 )

typedef uint32      uint;
typedef sint32      sint;
typedef float       float32;
typedef double      float64;

#if defined( _M_X64 ) || defined( __LP64__ )
typedef int64       intptr;
#else
typedef int32       intptr;
#endif

/*
    Macros
*/
#ifndef NULL
    #define NULL 0
#endif

#ifndef nullptr
    #define nullptr NULL
#endif

    #ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }
#endif

#ifndef SAFE_DELETE
    #define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
    #define SAFE_DELETE_ARRAY(p) if(p) { delete [] p; p = NULL; }
#endif

#ifndef ARRAY_LENGTH
    #define ARRAY_LENGTH( a ) (sizeof(a) / sizeof(a[0]) )
#endif

/*
    Various global enums
*/

enum BitField
{
    kBit0 =  0x00000001,
    kBit1 =  0x00000002,
    kBit2 =  0x00000004,
    kBit3 =  0x00000008,
    kBit4 =  0x00000010,
    kBit5 =  0x00000020,
    kBit6 =  0x00000040,
    kBit7 =  0x00000080,
    kBit8 =  0x00000100,
    kBit9 =  0x00000200,
    kBit10 = 0x00000400,
    kBit11 = 0x00000800,
    kBit12 = 0x00001000,
    kBit13 = 0x00002000,
    kBit14 = 0x00004000,
    kBit15 = 0x00008000,
    kBit16 = 0x00010000,
    kBit17 = 0x00020000,
    kBit18 = 0x00040000,
    kBit19 = 0x00080000,
    kBit20 = 0x00100000,
    kBit21 = 0x00200000,
    kBit22 = 0x00400000,
    kBit23 = 0x00800000,
    kBit24 = 0x01000000,
    kBit25 = 0x02000000,
    kBit26 = 0x04000000,
    kBit27 = 0x08000000,
    kBit28 = 0x10000000,
    kBit29 = 0x20000000,
    kBit30 = 0x40000000,
    kBit31 = 0x80000000,
};

inline bool CheckBit( uint bit_field, uint bit_mask )
{
    return (bit_field & bit_mask) == 1;
}
