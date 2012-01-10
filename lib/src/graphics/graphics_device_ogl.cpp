/*
 *  graphics_device.cpp
 *  engine
 *
 *  Created by Kyle C Weicht on 11/3/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "graphics_device.h"
#include "build.h"

#if BUILD_PLATFORM_ID == BUILD_PLATFORM_WINDOWS
    #include <Windows.h>
    #include "gl3/gl3.h"
#else
    #include <OpenGL/OpenGL.h>
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif

#include "system_ogl.h"

namespace
{


using namespace GraphicsDevice;
/*******************************************************************\
 Internal constants and types
\*******************************************************************/

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/
void InitializeOpenGL(void* window)
{
    /* Create the OS-specific context */
    SystemOpenGL::Initialize(window);

    /* Perform OpenGL setup */
}
void ShutdownOpenGL(void)
{
}
void FrameOpenGL(void)
{
}
void SetClearColorOpenGL(float r,float g,float b,float a,float depth)
{
    glClearColor(r,g,b,a);
    glClearDepth(depth);
}
void BeginFrameOpenGL(void)
{
}
void ClearOpenGL(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void PresentOpenGL(void)
{
    SystemOpenGL::BufferSwap();
}
void EndFrameOpenGL(void)
{
}

Shader CreateVertexShaderOpenGL(const char*)
{
    Shader shader = {nullptr}; return shader;
}
Shader CreatePixelShaderOpenGL(const char*)
{
    Shader shader = {nullptr}; return shader;
}
Material CreateMaterialOpenGL(const Shader&, const Shader&)
{
    Material material = {{0}}; return material;
}

}

namespace GraphicsDevice
{

/*******************************************************************\
 External variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/
void CreateOpenGLDevice(void* window)
{
    Initialize      = &InitializeOpenGL;
    Shutdown        = &ShutdownOpenGL;
    BeginFrame      = &BeginFrameOpenGL;
    Clear           = &ClearOpenGL;
    Present         = &PresentOpenGL;
    EndFrame        = &EndFrameOpenGL;
    SetClearColor   = &SetClearColorOpenGL;
    
    CreateVertexShader  = &CreateVertexShaderOpenGL;
    CreatePixelShader   = &CreatePixelShaderOpenGL;
    CreateMaterial      = &CreateMaterialOpenGL;
}

} // namespace GraphicsDevice;
