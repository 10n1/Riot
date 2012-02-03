/*
 * windows_openglDevice.c
 * graphicsDevice
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "graphicsDevice.h"
#include "openglDevice.h"

#if (GFX_API == GFX_OPENGL)

/* C headers */
#include <stdio.h>
#include <stdlib.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"

static GLenum _glError;
#define CheckGLError() \
    _glError = glGetError(); \
    assert(_glError == GL_NO_ERROR)

/*******************************************************************\
Internal Constants And types
\*******************************************************************/


/*******************************************************************\
Internal variables
\*******************************************************************/

/*******************************************************************\
Internal functions
\*******************************************************************/


/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
graphics_t* gfxCreate(void* window)
{
    graphics_t* device  = NULL;
    HDC     hDC         = NULL;
    HGLRC   hGLRC       = NULL;
    HGLRC   newContext = NULL;
    HWND    hWnd        = (HWND)(window);
    int     pixelFormat = 0;
    BOOL    success     = 1;
    GLenum  error       = GL_NO_ERROR;
	PIXELFORMATDESCRIPTOR   pfd;

    GLint pCreationAttributes[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,
        0,
    };
    
    /*
     * Create context to load extensions
     */
    hDC = GetDC(hWnd);
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize  = sizeof(pfd);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    assert(pixelFormat != 0);

    success = SetPixelFormat(hDC, pixelFormat, &pfd);
    assert(success);

    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);
    CheckGLError();

    /* Glew */
    error = glewInit();
    CheckGLError();
    if(GLEW_OK != error)
    {
        /* Glew init failed...not sure what happened */
        printf("Error: %s\n", glewGetErrorString(error));
        assert(0);
    }
    
    /*
     * Create new OpenGL context
     */
    assert(wglewIsSupported("WGL_ARB_create_context") == 1);

    newContext = wglCreateContextAttribsARB(hDC, 0, pCreationAttributes);
    assert(hGLRC);

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hGLRC);
    wglMakeCurrent(hDC, newContext);
    hGLRC = newContext;

    /*
     * Create the device
     */
    assert(device == NULL);
    device = malloc(sizeof(*device));
    memset(device, 0, sizeof(*device));

    device->dc = hDC;
    device->rc = hGLRC;

    CheckGLError();
    glFrontFace(GL_CW);
    CheckGLError();
    glCullFace(GL_BACK);
    CheckGLError();
    glEnable(GL_CULL_FACE);
    CheckGLError();

    return device;
}
void gfxPresent(graphics_t* device)
{
    SwapBuffers(device->dc);
    UNUSED_PARAMETER(device);
}
void gfxResize(graphics_t* device, int width, int height)
{
    UNUSED_PARAMETER(device);
    glViewport(0,0,width,height);
    CheckGLError();
}

#endif // #if (GD_API == GD_OPENGL)
