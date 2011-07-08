//
//  assert.h
//  Cosmos
//
//  Created by Kyle Weicht on 7/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "config.h"

/*
    Define debugBreak
*/
#if defined( _MSC_VER )
    #define debugBreak() __debugbreak()
#elif defined( __GNUC__ )
    #define debugBreak() __asm__( "int $3\n" : : )
#else
    #error Unsupported compiler
#endif

/*
    Define ASSERT
*/
#ifdef DEBUG
    #define ASSERT(expression)  \
        if(expression) { }      \
        else { debugBreak(); } 
#else
    #define ASSERT(expression)
#endif
