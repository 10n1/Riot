/*
 *  renderEngine.cpp
 *  renderEngineLib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#include "renderEngine.h"
#include "renderEngineInternal.h"
#include "graphicsDevice.h"

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

#include <memory.h>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace
{

/*******************************************************************\
 Internal constants and types
\*******************************************************************/
enum
{
    kMaxShaders = 8,
    kMaxMeshes  = 8,
};

struct mesh_t
{
    GraphicsDevice::buffer_t        vertexBuffer;
    GraphicsDevice::buffer_t        indexBuffer;
    GraphicsDevice::index_format_e  indexFormat;
    int indexCount;
};

struct render_t
{
    GraphicsDevice::shader_t shaders[kMaxShaders];
    mesh_t  meshes;

    uint64_t    numShaders;
    uint64_t    numMeshes;
}* s_render = nullptr;

/*******************************************************************\
 Internal variables
\*******************************************************************/

/*******************************************************************\
 Internal functions
\*******************************************************************/

} // namespace

namespace Render
{

/*******************************************************************\
 External variables
\*******************************************************************/
const size_t kRenderEngineSize = sizeof(render_t);

/*******************************************************************\
 External functions
\*******************************************************************/
void Initialize(void* window, void* memoryBuffer, size_t bufferSize)
{
    // Create the device
    assert(s_render == nullptr);
    assert(bufferSize >= kRenderEngineSize);
    s_render = (render_t*)memoryBuffer;
    memset(s_render, 0, kRenderEngineSize);

    // Initialization
    GraphicsDevice::Initialize(window);

    // Pipeline setup
    GraphicsDevice::SetClearColor(0.0f, 0.3f, 0.4f, 1.0f);
    GraphicsDevice::SetClearDepth(1.0f);

    int x = sizeof(render_t);
    x = sizeof(s_render);
}
void Shutdown(void)
{
    GraphicsDevice::Shutdown();
}
void Frame(void)
{
    GraphicsDevice::Present();
    GraphicsDevice::Clear();
}

shader_t CreateShader(const char* shaderSource, shader_type_e type)
{
    GraphicsDevice::shader_t shader;
    switch(type)
    {
    case kVertexShader:
        shader = GraphicsDevice::CreateVertexShader(shaderSource);
        break;
    case kPixelShader:
        shader = GraphicsDevice::CreatePixelShader(shaderSource);
        break;
    default:
        assert(0);
    }

    assert(s_render->numShaders < kMaxShaders);
    const shader_t shaderId = s_render->numShaders++;
    s_render->shaders[shaderId] = shader;

    return shaderId;
}
mesh_t CreateMesh(  vertex_layout_e layout, 
                    int indexCount, 
                    int vertexCount, 
                    size_t vertexSize, 
                    size_t indexSize, 
                    const void* vertices, 
                    const void* indices)
{
    const GraphicsDevice::buffer_t vertexBuffer = GraphicsDevice::CreateVertexBuffer(vertexSize*vertexCount, vertices);
    const GraphicsDevice::buffer_t indexBuffer = GraphicsDevice::CreateIndexBuffer(indexSize*indexCount, indices);

    return mesh_t();
}

} // namespace Render
