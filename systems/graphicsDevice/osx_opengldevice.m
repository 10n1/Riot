/*
 *  osx_opengldevice.m
 *  graphicsDevice
 *
 *  Created by Kyle C Weicht on 1/16/12.
 *  Copyright (c) 2012 Kyle C Weicht. All rights reserved.
 */
#include "opengldevice.h"

/* C headers */
#if defined(WIN32)
    #include "glew/gl/glew.h"
    #include "glew/gl/wglew.h"
#elif defined(__APPLE__)
    #include <Cocoa/Cocoa.h>
    #include <OpenGL/OpenGL.h>
#endif
/* C++ headers */
/* External headers */
/* Internal headers */

/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/
static void CheckGLError(void)
{
    GLenum  error = glGetError();
    assert(error == GL_NO_ERROR);
}

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
graphics_t* gfxCreate(void* window)
{
    NSWindow*               nsWindow        = (NSWindow*)window;
    graphics_t*             device          = NULL;
    NSOpenGLContext*        cocoaContext    = NULL;
    NSOpenGLPixelFormat*    cocoaPixelFormat= NULL;
    NSView*                 view            = [nsWindow contentView];
    GLint                   vsyncInterval   = 0;
    
    NSOpenGLPixelFormatAttribute attributes[] = 
    {
#if (MAC_OS_X_VERSION_MAX_ALLOWED == MAC_OS_X_VERSION_10_7) && 1
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
#endif
        NSOpenGLPFAAccelerated,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFADoubleBuffer,   
        NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)32,
        NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)24,
        (NSOpenGLPixelFormatAttribute)0,
    };
    
    cocoaPixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
    assert(cocoaPixelFormat);
    
    cocoaContext = [[NSOpenGLContext alloc] initWithFormat:cocoaPixelFormat shareContext:nil];
    assert(cocoaContext);
    
    [cocoaContext setView:view];
    CheckGLError();
    [cocoaContext makeCurrentContext];
    CheckGLError();
    [cocoaContext setValues:&vsyncInterval forParameter:NSOpenGLCPSwapInterval];
    CheckGLError();
    
    /* Load extensions */
    // TODO: Using glew in OS X actually makes things not work...
    //error = glewInit();
    //if(GLEW_OK != error)
    //{
    //   /* Glew init failed...not sure what happened */
    //   printf("Error: %s\n", glewGetErrorString(error));
    //   assert(0);
    //}
    //glGetError(); /* glewInit causes error 1280, so we remove it from the queue */
    //CheckGLError();
    
    /*
     * Create the device
     */
    assert(device == NULL);
    device = malloc(sizeof(*device));
    memset(device, 0, sizeof(*device));
    device->context = cocoaContext;
    device->pixelFormat = cocoaPixelFormat;
    
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
    [(NSOpenGLContext*)device->context flushBuffer];
}
