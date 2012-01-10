/*
 *  render_engine.cpp
 *  engine
 *
 *  Created by Kyle Weicht on 10/19/11.
 *  Copyright (c) 2011 Kyle Weicht. All rights reserved.
 */
#include "graphics/render_engine.h"

#include <string.h>
#include <memory.h>

#include "graphics_device.h"
#include "system/memory.h"
#include "assert.h"

namespace
{

using namespace RenderEngine;

using GraphicsDevice::material_t;
using GraphicsDevice::shader_t;

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
enum
{
    kMaxViews = 8,
    kMaxShaders = 16,
    kMaxMaterials = 16,
};

struct Engine
{
    shader_t    shaders[kMaxShaders];
    uint64      numShaders;
    material_t  materials[kMaxMaterials];
    uint64      numMaterials;
};

/*******************************************************************\
 Internal variables
\*******************************************************************/
Engine* s_engine = nullptr;

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace RenderEngine
{

/*******************************************************************\
 External variables
\*******************************************************************/
RenderStatus    g_status = RenderStatus::kUninitialized;

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void)
{
    /* Create the render engine */
    s_engine = (Engine*)Memory::GlobalAllocator()->Allocate(sizeof(Engine));
    memset(s_engine, 0, sizeof(Engine));
    
    CreateDevice(nullptr, GraphicsDeviceType::kNull);
    g_status = RenderStatus::kOk;
}
void CreateDevice(void* window, GraphicsDeviceType type)
{
    /* Create the device */
    switch(type)
    {
    case GraphicsDeviceType::kNull:
        GraphicsDevice::CreateNullDevice(window);
        g_status = RenderStatus::kOk;
        break;
    case GraphicsDeviceType::kOpenGL:
        GraphicsDevice::CreateOpenGLDevice(window);
        g_status = RenderStatus::kOk;
        break;    
    default:
        assert(0);
        break;
    }
    
    /* initialize */
    GraphicsDevice::Initialize(window);
    GraphicsDevice::SetClearColor(0.0f, 0.3f, 0.4f, 1.0f, 1.0f);
}
void Shutdown(void)
{
    GraphicsDevice::Shutdown();
    g_status = RenderStatus::kUninitialized;
}
void Frame(void)
{
    GraphicsDevice::BeginFrame();
    GraphicsDevice::Clear();
    
    GraphicsDevice::Present();    
    GraphicsDevice::EndFrame();
}
shader_id_t CreateVertexShader(const char* source)
{
    const shader_t shader = GraphicsDevice::CreateVertexShader(source);
    const int shaderIndex = s_engine->numShaders++;
    s_engine->shaders[shaderIndex] = shader;

    return shaderIndex;
}
shader_id_t CreatePixelShader(const char* source)
{
    const shader_t shader = GraphicsDevice::CreatePixelShader(source);
    const int shaderIndex = s_engine->numShaders++;
    s_engine->shaders[shaderIndex] = shader;

    return shaderIndex;
}

} // namespace RenderEngine
