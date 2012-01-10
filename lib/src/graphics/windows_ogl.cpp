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

// OpenGl functions
PFNGLENABLEPROC     glEnable     = nullptr;
PFNGLDISABLEPROC    glDisable    = nullptr;
PFNGLCLEARPROC      glClear      = nullptr;
PFNGLCLEARCOLORPROC glClearColor = nullptr;
PFNGLCLEARDEPTHPROC glClearDepth = nullptr;
PFNGLDRAWBUFFERPROC glDrawBuffer = nullptr;
PFNGLDRAWARRAYSPROC glDrawArrays = nullptr;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = nullptr;
PFNGLVIEWPORTPROC   glViewport   = nullptr;
PFNGLFRONTFACEPROC  glFrontFace  = nullptr;
PFNGLDEPTHFUNCPROC  glDepthFunc  = nullptr;
PFNGLDEPTHMASKPROC  glDepthMask  = nullptr;
PFNGLDEPTHRANGEPROC glDepthRange = nullptr;
PFNGLBLENDFUNCPROC  glBlendFunc  = nullptr;
PFNGLBLENDEQUATIONPROC  glBlendEquation  = nullptr;
PFNGLGENTEXTURESPROC glGenTextures = nullptr;
PFNGLTEXIMAGE2DPROC glTexImage2D = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLGETERRORPROC   glGetError   = nullptr;
PFNGLPOLYGONMODEPROC    glPolygonMode = nullptr;


// OpenGL extensions (OpenGL 1.2+)
PFNGLBINDFRAMEBUFFERPROC    glBindFramebuffer = nullptr;
PFNGLCREATESHADERPROC       glCreateShader = nullptr;
PFNGLDELETESHADERPROC       glDeleteShader = nullptr;
PFNGLCOMPILESHADERPROC      glCompileShader = nullptr;
PFNGLGETSHADERIVPROC        glGetShaderiv = nullptr;
PFNGLGETPROGRAMIVPROC       glGetProgramiv = nullptr;
PFNGLCREATEPROGRAMPROC      glCreateProgram = nullptr;
PFNGLATTACHSHADERPROC       glAttachShader = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;
PFNGLLINKPROGRAMPROC        glLinkProgram = nullptr;
PFNGLGETSHADERINFOLOGPROC   glGetShaderInfoLog = nullptr;
PFNGLGETPROGRAMINFOLOGPROC  glGetProgramInfoLog = nullptr;
PFNGLSHADERSOURCEPROC       glShaderSource = nullptr;


namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
#define GET_OPENGL_EXTENSION(type, ext) ext = (type)wglGetProcAddress(#ext); assert(ext)
#define LOAD_FROM_DLL(type, ext) ext = (type)GetProcAddress( s_openGL32, #ext ); assert( ext )

/*******************************************************************\
 Internal variables
\*******************************************************************/
HDC     s_hDC         = nullptr;
HGLRC   s_hGLRC       = nullptr;
HMODULE s_openGL32    = nullptr;
sint    s_pixelFormat = -1;

/*******************************************************************\
 Internal functions
\*******************************************************************/
PFNWGLCREATECONTEXTATTRIBSARBPROC    wglCreateContextAttribsARB = nullptr;
PFNWGLCHOOSEPIXELFORMATARBPROC       wglChoosePixelFormatARB = nullptr;
void LoadOpenGLExtensions(void)
{
    // Load the basic OpenGL calls from the DLL
    LOAD_FROM_DLL( PFNGLENABLEPROC    , glEnable     );
    LOAD_FROM_DLL( PFNGLDISABLEPROC   , glDisable    );
    LOAD_FROM_DLL( PFNGLCLEARPROC     , glClear      );
    LOAD_FROM_DLL( PFNGLCLEARCOLORPROC, glClearColor );
    LOAD_FROM_DLL( PFNGLCLEARDEPTHPROC, glClearDepth );
    LOAD_FROM_DLL( PFNGLDRAWBUFFERPROC, glDrawBuffer );
    LOAD_FROM_DLL( PFNGLDRAWARRAYSPROC, glDrawArrays );
    LOAD_FROM_DLL( PFNGLVIEWPORTPROC  , glViewport   );
    LOAD_FROM_DLL( PFNGLFRONTFACEPROC , glFrontFace  );
    LOAD_FROM_DLL( PFNGLDEPTHFUNCPROC,  glDepthFunc  );
    LOAD_FROM_DLL( PFNGLDEPTHMASKPROC,  glDepthMask  );
    LOAD_FROM_DLL( PFNGLDEPTHRANGEPROC, glDepthRange );
    LOAD_FROM_DLL( PFNGLBLENDFUNCPROC,  glBlendFunc  );
    LOAD_FROM_DLL( PFNGLGENTEXTURESPROC, glGenTextures );
    LOAD_FROM_DLL( PFNGLTEXIMAGE2DPROC, glTexImage2D );
    LOAD_FROM_DLL( PFNGLGETERRORPROC,   glGetError   );
    LOAD_FROM_DLL( PFNGLPOLYGONMODEPROC, glPolygonMode );

    // Load the rest from the driver
    GET_OPENGL_EXTENSION( PFNGLBINDFRAMEBUFFERPROC         , glBindFramebuffer );    
    GET_OPENGL_EXTENSION( PFNGLGENBUFFERSPROC, glGenBuffers );
    GET_OPENGL_EXTENSION( PFNGLBUFFERDATAPROC, glBufferData );
    GET_OPENGL_EXTENSION( PFNGLBINDBUFFERPROC, glBindBuffer );
    GET_OPENGL_EXTENSION( PFNGLBLENDEQUATIONPROC, glBlendEquation );
    GET_OPENGL_EXTENSION( PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced );
    GET_OPENGL_EXTENSION( PFNGLCREATESHADERPROC      , glCreateShader );
    GET_OPENGL_EXTENSION( PFNGLDELETESHADERPROC      , glDeleteShader );
    GET_OPENGL_EXTENSION( PFNGLCOMPILESHADERPROC     , glCompileShader );
    GET_OPENGL_EXTENSION( PFNGLGETSHADERIVPROC       , glGetShaderiv );
    GET_OPENGL_EXTENSION( PFNGLGETPROGRAMIVPROC      , glGetProgramiv );
    GET_OPENGL_EXTENSION( PFNGLCREATEPROGRAMPROC     , glCreateProgram );
    GET_OPENGL_EXTENSION( PFNGLATTACHSHADERPROC      , glAttachShader );
    GET_OPENGL_EXTENSION( PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation );
    GET_OPENGL_EXTENSION( PFNGLLINKPROGRAMPROC       , glLinkProgram );
    GET_OPENGL_EXTENSION( PFNGLGETSHADERINFOLOGPROC  , glGetShaderInfoLog );
    GET_OPENGL_EXTENSION( PFNGLGETPROGRAMINFOLOGPROC , glGetProgramInfoLog );
    GET_OPENGL_EXTENSION( PFNGLSHADERSOURCEPROC      , glShaderSource );
}

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

    s_openGL32 = LoadLibrary("OpenGL32.dll");
    assert(s_openGL32);
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
    hGLRC = nullptr;
    
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

    //////////////////////////////////////////
    // Create OpenGL context
    GLint pCreationAttributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        0,
    };
    hGLRC = wglCreateContextAttribsARB( hDC, 0, pCreationAttributes );

    assert(hGLRC);

    wglMakeCurrent(hDC, hGLRC);

    LoadOpenGLExtensions();

    s_hDC   = hDC;
    s_hGLRC = hGLRC;
}
void BufferSwap(void)
{
    SwapBuffers(s_hDC);
}

} // namespace GraphicsDevice;
