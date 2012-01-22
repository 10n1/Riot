/*
 *  osx_openglDevice.h
 *  graphicsDevice
 *
 *  Created by Kyle C Weicht on 1/16/12..
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#ifndef graphicsDevice_openglDevice_h
#define graphicsDevice_openglDevice_h

#include "graphicsDevice.h"

/* C headers */
#include <stdio.h>
#include <stdlib.h>
/* C++ headers */
/* External headers */
#if defined(WIN32)
    #include "glew/gl/glew.h"
    #include "glew/gl/wglew.h"
#elif defined(__APPLE__)
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl3.h>
#endif
/* Internal headers */

/*******************************************************************\
 External constants and types
\*******************************************************************/
#ifdef WIN32
struct graphics_t
{
    HDC     dc;
    HGLRC   rc;
};
#else
struct graphics_t
{
    void*  context;
    void*  pixelFormat;
};
#endif

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/

#endif /* include guard */
