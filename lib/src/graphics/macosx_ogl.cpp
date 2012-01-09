/*
 *  macosx_ogl.cpp
 *  lib
 *
 *  Created by Kyle C Weicht on 1/4/12.
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#include "system_ogl.h"

#include <OpenGL/OpenGL.h> 
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>

#include "assert.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
CGLContextObj        s_glContext     = nullptr;
CGLPixelFormatObj    s_glPixelFormat = nullptr;

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace SystemOpenGL
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void* window)
{CGLContextObj       context = NULL;
    CGLPixelFormatObj   pixel_format = NULL;
    GLint               vsync_interval = 0;
    
    CGLPixelFormatAttribute attributes[] = 
    {
#if (MAC_OS_X_VERSION_MAX_ALLOWED == MAC_OS_X_VERSION_10_7)
        kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
#endif
        kCGLPFAAccelerated,
        kCGLPFANoRecovery,
        kCGLPFADoubleBuffer,   
        kCGLPFAColorSize, (CGLPixelFormatAttribute)kCGL32Bit,
        kCGLPFADepthSize, (CGLPixelFormatAttribute)kCGL24Bit,
        (CGLPixelFormatAttribute)0,
    };
    
    GLint nPix = 0;
    CGLChoosePixelFormat(attributes, &pixel_format, &nPix);
    assert(pixel_format);
    CGLCreateContext(pixel_format, NULL, &context);
    assert(context);
     
    CGLSetParameter(context, kCGLCPSwapInterval, &vsync_interval);
    CGLSetCurrentContext(context);
    
    s_glContext     = context;
    s_glPixelFormat = pixel_format;
}

} // namespace SystemOpenGL
