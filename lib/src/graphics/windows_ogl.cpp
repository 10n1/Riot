/*
 *  graphics_device.cpp
 *  engine
 *
 *  Created by Kyle C Weicht on 11/3/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "system_ogl.h"
#include "build.h"
#include "assert.h"

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <Windows.h>
    #include "gl3/gl3.h"
#else
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif


namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
#define GET_OPENGL_EXTENSION(type, ext) ext = (type)wglGetProcAddress(#ext); assert(ext)

/*******************************************************************\
 Internal variables
\*******************************************************************/
HDC     s_hDC         = nullptr;
HGLRC   s_hGLRC       = nullptr;
sint    s_pixelFormat = -1;
HMODULE s_openGL32    = nullptr;

/*******************************************************************\
 Internal functions
\*******************************************************************/
PFNWGLCREATECONTEXTATTRIBSARBPROC    wglCreateContextAttribsARB = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC       wglChoosePixelFormatARB = nullptr;

}

namespace SystemOpenGL
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void* window)
{
    HDC     hDC = NULL;
    HGLRC   hGLRC = NULL;
    HWND    hWnd = static_cast<HWND>(window);
    sint    pixelFormat = 0;

    //////////////////////////////////////////
    // Create a rendering context
    hDC = GetDC(hWnd);      
    PIXELFORMATDESCRIPTOR pfd;
    pixelFormat = 1;

    bool bSuccess = SetPixelFormat(hDC, pixelFormat, &pfd);
    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);

    // Load the creation functions we need
    GET_OPENGL_EXTENSION(PFNWGLCREATECONTEXTATTRIBSARBPROC, wglCreateContextAttribsARB);
    GET_OPENGL_EXTENSION(PFNWGLCHOOSEPIXELFORMATARBPROC   , wglChoosePixelFormatARB);

    // Release and start over
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hGLRC);
    
    //////////////////////////////////////////
    // Define attributes required
    uint pixCount = 0;
    int pixAttribs[] = 
    {
        WGL_SUPPORT_OPENGL_ARB, 1,
        WGL_DRAW_TO_WINDOW_ARB, 1,
        WGL_ACCELERATION_ARB,   1,
        WGL_COLOR_BITS_ARB,     32,
        WGL_DEPTH_BITS_ARB,     24,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        //WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        //WGL_SAMPLES_ARB,        8,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
        0
    };

    // Chose most appropriate format
    wglChoosePixelFormatARB(hDC, &pixAttribs[0], NULL, 1, &pixelFormat, &pixCount);

    if(pixelFormat == -1)
    {
        // TODO: Handle error
        assert(0);
    }

    // Set pixel format
    SetPixelFormat(hDC, pixelFormat, &pfd);
}

} // namespace GraphicsDevice;
