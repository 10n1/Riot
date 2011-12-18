/*
 *  graphics_device.cpp
 *  engine
 *
 *  Created by Kyle C Weicht on 11/3/11.
 *  Copyright (c) 2011 Kyle C Weicht. All rights reserved.
 */

#include "graphics_device.h"

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
void InitializeOpenGL(void*)
{
}
void ShutdownOpenGL(void)
{
}
void FrameOpenGL(void)
{
}
void SetClearColorOpenGL(float,float,float,float,float)
{
}
void BeginFrameOpenGL(void)
{
}
void ClearOpenGL(void)
{
}
void PresentOpenGL(void)
{
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
    Material material = {0}; return material;
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
