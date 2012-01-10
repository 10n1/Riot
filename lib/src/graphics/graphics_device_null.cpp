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
void InitializeNull(void*)
{
}
void ShutdownNull(void)
{
}
void FrameNull(void)
{
}
void SetClearColorNull(float,float,float,float,float)
{
}
void BeginFrameNull(void)
{
}
void ClearNull(void)
{
}
void PresentNull(void)
{
}
void EndFrameNull(void)
{
}

shader_t CreateVertexShaderNull(const char*)
{
    shader_t shader = {nullptr}; return shader;
}
shader_t CreatePixelShaderNull(const char*)
{
    shader_t shader = {nullptr}; return shader;
}
material_t CreateMaterialNull(const shader_t&, const shader_t&)
{
    material_t material = {{0}}; return material;
}

}

namespace GraphicsDevice
{

/*******************************************************************\
 External variables
\*******************************************************************/
void (*Initialize)(void* window)    = nullptr;
void (*Shutdown)(void)              = nullptr;
void (*BeginFrame)(void)            = nullptr;
void (*Clear)(void)                 = nullptr;
void (*Present)(void)               = nullptr;
void (*EndFrame)(void)              = nullptr;
void (*SetClearColor)(float,float,float,float,float) = nullptr;
shader_t (*CreateVertexShader)(const char*)   = nullptr;
shader_t (*CreatePixelShader)(const char*)    = nullptr;
material_t (*CreateMaterial)(const shader_t&, const shader_t&) = nullptr;

/*******************************************************************\
 External functions
\*******************************************************************/
void CreateNullDevice(void* window)
{
    Initialize      = &InitializeNull;
    Shutdown        = &ShutdownNull;
    BeginFrame      = &BeginFrameNull;
    Clear           = &ClearNull;
    Present         = &PresentNull;
    EndFrame        = &EndFrameNull;
    SetClearColor   = &SetClearColorNull;
    
    CreateVertexShader = &CreateVertexShaderNull;
    CreatePixelShader = &CreatePixelShaderNull;
    CreateMaterial = &CreateMaterialNull;
}

} // namespace GraphicsDevice;
