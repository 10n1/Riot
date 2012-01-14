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
    kMaxMaterials = 8,
    kMaxRenderCommands = 8,
    kMaxTextures = 8,
};

struct material_t
{
    GraphicsDevice::shader_t    vertexShader;
    GraphicsDevice::shader_t    pixelShader;
    GraphicsDevice::program_t   program;
};

struct render_command_t
{
    material_t  material;
    GraphicsDevice::mesh_t      mesh;
};

struct render_t
{
    GraphicsDevice::vertex_format_t vertexFormats[Render::kMAX_VERTEX_LAYOUTS];
    GraphicsDevice::shader_t        shaders[kMaxShaders];
    material_t                      materials[kMaxMaterials];
    GraphicsDevice::mesh_t          meshes[kMaxMeshes];
    render_command_t                renderCommands[kMaxRenderCommands];
    GraphicsDevice::texture_t       textures[kMaxTextures];

    int numShaders;
    int numMeshes;
    int numMaterials;
    int numRenderCommands;
    int numTextures;
}* s_render = nullptr;

/*******************************************************************\
 Internal variables
\*******************************************************************/
static const GraphicsDevice::vertex_layout_t kShaderLayouts[Render::kMAX_VERTEX_LAYOUTS][16] =
{
    // kPos
    {
        { GraphicsDevice::kShaderInputPosition, 3 },
        { GraphicsDevice::kShaderInputNull,     0 },
    },    
    // kPosTex
    {
        { GraphicsDevice::kShaderInputPosition,     3 },
        { GraphicsDevice::kShaderInputTexCoord0,    2 },
        { GraphicsDevice::kShaderInputNull,         0 },
    },
};

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

    // Create vertex formats
    //for(int ii=0; ii<kMAX_VERTEX_LAYOUTS; ++ii)
    //{
    //    s_render->vertexFormats[ii] = GraphicsDevice::CreateVertexLayout(kShaderLayouts[ii], 3*sizeof(float));
    //}
}
void Shutdown(void)
{
    GraphicsDevice::Shutdown();

    // Clean up the memory
    memset(s_render, 0, kRenderEngineSize);
    s_render = nullptr;
}
void Frame(void)
{
    GraphicsDevice::Clear();
    for(int ii=0; ii<s_render->numRenderCommands; ++ii)
    {
        const render_command_t& command = s_render->renderCommands[ii];

        GraphicsDevice::SetProgram(command.material.program);
        //GraphicsDevice::SetVertexLayout(command.mesh.vertexFormat);
        //GraphicsDevice::BindIndexBuffer(command.mesh.indexBuffer);
        //GraphicsDevice::BindVertexBuffer(command.mesh.vertexBuffer);
        GraphicsDevice::DrawMesh(command.mesh);
    }
    s_render->numRenderCommands = 0;
    GraphicsDevice::Present();
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
material_t CreateMaterial(shader_t vertexShader, shader_t pixelShader)
{
    GraphicsDevice::shader_t deviceVertexShader = s_render->shaders[vertexShader];
    GraphicsDevice::shader_t devicePixelShader = s_render->shaders[pixelShader];

    GraphicsDevice::program_t program = GraphicsDevice::CreateProgram(deviceVertexShader, devicePixelShader);
    ::material_t material = 
    {
        deviceVertexShader,
        devicePixelShader,
        program
    };

    assert(s_render->numMaterials < kMaxMaterials);
    const material_t materialId = s_render->numMaterials++;
    s_render->materials[materialId] = material;

    return materialId;
}
mesh_t CreateMesh(  vertex_layout_e layout, 
                    int indexCount, 
                    int vertexCount, 
                    size_t vertexSize, 
                    size_t indexSize, 
                    const void* vertices, 
                    const void* indices)
{
    GraphicsDevice::mesh_t mesh = GraphicsDevice::CreateMesh(kShaderLayouts[layout],
                                                             indexCount, 
                                                             vertexCount, 
                                                             vertexSize, 
                                                             indexSize, 
                                                             vertices, 
                                                             indices);
                                                            

    assert(s_render->numMeshes < kMaxMeshes);
    const mesh_t meshId = s_render->numMeshes++;
    s_render->meshes[meshId] = mesh;

    return meshId;
}
texture_t CreateTexture(int width, int height, int bits, void* data)
{
    GraphicsDevice::texture_t texture = GraphicsDevice::CreateTexture(width, height, bits, data);
    
    assert(s_render->numTextures < kMaxTextures);
    const texture_t textureId = s_render->numTextures++;
    s_render->textures[textureId] = texture;

    return textureId;
}
void SubmitCommand(material_t material, mesh_t mesh)
{
    int renderCommandId = s_render->numRenderCommands++;

    s_render->renderCommands[renderCommandId].material = s_render->materials[material];
    s_render->renderCommands[renderCommandId].mesh = s_render->meshes[mesh];
}

void UpdateTextureData(texture_t texture, int width, int height, int bits, void* data)
{
    GraphicsDevice::UpdateTextureData(s_render->textures[texture], width, height, bits, data);
}

} // namespace Render
