/*
 *  system_ogl.h
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#ifndef engine_system_ogl_h_
#define engine_system_ogl_h_

#include "build.h"
#include "types.h"

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include "gl3/gl3.h"
    #include "gl/wglext.h"
#endif

namespace SystemOpenGL
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
void Initialize(void* window);

}

#endif
