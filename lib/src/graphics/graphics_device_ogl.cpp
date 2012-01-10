/*
 *  graphics_device.cpp
 *  engine
 *
 *  Created by Kyle C Weicht on 11/3/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "graphics_device.h"
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

#include <stdio.h>

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
GLuint CompileShader(GLenum shaderType, const char* shaderSource)
{   
    GLchar  statusBuffer[1024] = {0};
    GLint   status = GL_TRUE;
    GLuint  shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE)
    {
        glGetShaderInfoLog(shader, sizeof(statusBuffer), NULL, statusBuffer);
        printf("Error:\t%s\n", statusBuffer);
        assert(0);
    }
    
    return shader;
}

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

shader_t CreateVertexShaderOpenGL(const char* source)
{
    const GLuint shaderIndex = CompileShader(GL_VERTEX_SHADER, source);
    shader_t shader;
    shader.intShader = shaderIndex;

    return shader;
}
shader_t CreatePixelShaderOpenGL(const char* source)
{
    const GLuint shaderIndex = CompileShader(GL_FRAGMENT_SHADER, source);
    shader_t shader;
    shader.intShader = shaderIndex;

    return shader;
}
material_t CreateMaterialOpenGL(const shader_t&, const shader_t&)
{
    material_t material = {{0}}; return material;
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
