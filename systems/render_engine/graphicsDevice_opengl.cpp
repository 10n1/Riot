/*
 *  graphicsDevice_opengl.cpp
 *  renderEngine
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#include "graphicsDevice.h"
#include "renderEngineInternal.h"

/*
 * Platform detection
 */
#define BUILD_PLATFORM_WINDOWS 1
#define BUILD_PLATFORM_MACOS   2
#define BUILD_PLATFORM_LINUX   3
#define BUILD_PLATFORM_IOS     4
#define BUILD_PLATFORM_ANDROID 5

#if defined( WIN32 )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#elif defined( __MACH__ )
    #include <TargetConditionals.h>
    #if( TARGET_OS_IPHONE )
        #define BUILD_PLATFORM_ID BUILD_PLATFORM_IOS
    #else
        #define BUILD_PLATFORM_ID BUILD_PLATFORM_MACOS
    #endif
#elif defined( __ANDROID__ )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_ANDROID
#elif defined( __linux__ )
    #define BUILD_PLATFORM_ID BUILD_PLATFORM_LINUX
#else
    #error No valid target found
#endif


#include "glew/gl/glew.h"
#include "glew/gl/wglew.h"
#include <stdio.h>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/
HDC     s_hDC         = nullptr;
HGLRC   s_hGLRC       = nullptr;
int     s_pixelFormat = -1;

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace GraphicsDevice
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
void Initialize(void* window)
{
    HDC     hDC = NULL;
    HGLRC   hGLRC = NULL;
    HWND    hWnd = static_cast<HWND>(window);
    int     pixelFormat = 0;
    
    //
    // Create context to load extensions
    //
    hDC = GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd;
    pixelFormat = 1;

    BOOL bSuccess = SetPixelFormat(hDC, pixelFormat, &pfd);
    assert(bSuccess);
    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);

    // Initialize Glew
    GLenum error = glewInit();
    if(GLEW_OK != error)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("Error: %s\n", glewGetErrorString(error));
        assert(0);
    }

    // Release and start over
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hGLRC);
    hGLRC = nullptr;

    //
    // Define attributes required
    //
    unsigned int pixCount = 0;
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

    //
    // Create OpenGL context
    //
    GLint pCreationAttributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        0,
    };
    hGLRC = wglCreateContextAttribsARB( hDC, 0, pCreationAttributes );
    assert(hGLRC);
    wglMakeCurrent(hDC, hGLRC);

    s_hDC   = hDC;
    s_hGLRC = hGLRC;
    s_pixelFormat = pixelFormat;
}
void Shutdown(void)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(s_hGLRC);
    s_hGLRC = nullptr;
}
#else
    #error Need OpenGL
#endif // BUILD_PLATFORM_ID

// Pipeline control
void SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
}
void SetClearDepth(float d)
{
    glClearDepth(d);
}

// Frame controls
void Clear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Present(void)
{
#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    SwapBuffers(s_hDC);
#else
    #error Need OpenGL
#endif // BUILD_PLATFORM_ID
}

} // namespace GraphicsDevice
