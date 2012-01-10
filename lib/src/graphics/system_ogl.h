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


#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    // OpenGl functions
    extern PFNGLENABLEPROC          glEnable;
    extern PFNGLDISABLEPROC         glDisable;
    extern PFNGLCLEARPROC           glClear;
    extern PFNGLCLEARCOLORPROC      glClearColor;
    extern PFNGLCLEARDEPTHPROC      glClearDepth;
    extern PFNGLDRAWBUFFERPROC      glDrawBuffer;
    extern PFNGLDRAWARRAYSPROC      glDrawArrays;
    extern PFNGLDRAWARRAYSINSTANCEDPROC      glDrawArraysInstanced;
    extern PFNGLVIEWPORTPROC        glViewport;
    extern PFNGLFRONTFACEPROC       glFrontFace;
    extern PFNGLDEPTHFUNCPROC       glDepthFunc;
    extern PFNGLDEPTHMASKPROC       glDepthMask;
    extern PFNGLDEPTHRANGEPROC      glDepthRange;
    extern PFNGLBLENDFUNCPROC       glBlendFunc;
    extern PFNGLBLENDEQUATIONPROC   glBlendEquation;
    extern PFNGLGENTEXTURESPROC     glGenTextures;
    extern PFNGLTEXIMAGE2DPROC      glTexImage2D;
    extern PFNGLGENBUFFERSPROC      glGenBuffers;
    extern PFNGLBUFFERDATAPROC      glBufferData;
    extern PFNGLBINDBUFFERPROC      glBindBuffer;
    extern PFNGLGETERRORPROC        glGetError;
    extern PFNGLPOLYGONMODEPROC     glPolygonMode;

    // OpenGL extensions (OpenGL 1.2+)
    extern PFNGLBINDFRAMEBUFFERPROC     glBindFramebuffer;
    extern PFNGLCREATESHADERPROC        glCreateShader;
    extern PFNGLDELETESHADERPROC        glDeleteShader;
    extern PFNGLCOMPILESHADERPROC       glCompileShader;
    extern PFNGLGETSHADERIVPROC         glGetShaderiv;
    extern PFNGLGETPROGRAMIVPROC        glGetProgramiv;
    extern PFNGLCREATEPROGRAMPROC       glCreateProgram;
    extern PFNGLATTACHSHADERPROC        glAttachShader;
    extern PFNGLBINDATTRIBLOCATIONPROC  glBindAttribLocation;
    extern PFNGLLINKPROGRAMPROC         glLinkProgram;
    extern PFNGLGETSHADERINFOLOGPROC    glGetShaderInfoLog;
    extern PFNGLGETPROGRAMINFOLOGPROC   glGetProgramInfoLog;
    extern PFNGLSHADERSOURCEPROC        glShaderSource;
#endif // BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS

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
void BufferSwap(void);

}

#endif
