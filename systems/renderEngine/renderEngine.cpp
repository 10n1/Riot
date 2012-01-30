/*
 * renderEngine.cpp
 * renderEngine
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "renderEngine.h"

/* C headers */
#include <stdlib.h>
#include <memory.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "../graphicsDevice/graphicsDevice.h"

/*
 * Define debugBreak
 */
#if defined( _MSC_VER )
    #define debugBreak() __debugbreak()
#elif defined( __GNUC__ )
    #define debugBreak() __asm__( "int $3\n" : : )
#else
    #error Unsupported compiler
#endif

/*
 * Define assert
 */
#ifndef assert
    #define assert(condition)   \
        do                      \
        {                       \
            if(!(condition))    \
            {                   \
                debugBreak();   \
            }                   \
        } while(__LINE__ == -1)
        /* This causes warning 4127 with VC++ (conditional expression is constant) */
    #if defined( _MSC_VER )
        #pragma warning(disable:4127)
    #endif /* defined( _MSC_VER ) */
#endif

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
enum
{
    kMaxMaterials = 16,
    kMaxMeshes = 16,
    kMaxTextures = 16,
    kMaxCommands = 1024,
    
    kConstantBufferSize = sizeof(float)*16
};
struct render_command_t
{
    float           worldMatrix[16];
    float           viewProj[16];
    material_id_t   material;
    texture_id_t    texture;
    mesh_id_t       mesh;
};
struct render_t
{
    graphics_t* graphics;

    vertex_shader_t*    vertexShaders[kMaxMaterials];
    int                 numVertexShaders;
    pixel_shader_t*     pixelShaders[kMaxMaterials];
    int                 numPixelShaders;
    material_t*         materials[kMaxMaterials];
    int                 numMaterials;
    mesh_t*             meshes[kMaxMeshes];
    int                 numMeshes;
    texture_t*          textures[kMaxTextures];
    int                 numTextures;
    constant_buffer_t*  constantBuffers[16];
    
    render_command_t    renderCommands[kMaxCommands];
    int                 numRenderCommands;
}* s_render;

/*******************************************************************\
Internal variables
\*******************************************************************/
const vertex_element_desc_t kVertexFormats[kMAX_VERTEX_FORMATS][16] =
{
    { /* kVtxFmtPosTex */
        { kGfxShaderInputPosition,  3 },
        { kGfxShaderInputTexCoord0, 2 },
        { kGfxShaderInputNull,      0 },
    },
    { /* kVtxFmtPosNormTex */
        { kGfxShaderInputPosition,  3 },
        { kGfxShaderInputNormal,    3 },
        { kGfxShaderInputTexCoord0, 2 },
        { kGfxShaderInputNull,      0 },
    }
};

/*******************************************************************\
Internal functions
\*******************************************************************/

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void renderInit(graphics_t* graphics)
{
    /* Allocate structure */
    s_render = (render_t*)malloc(sizeof(*s_render));
    memset(s_render, 0, sizeof(*s_render));
    s_render->graphics = graphics;

    /* Fill it out */
    for(int ii=0; ii<16; ++ii)
    {
        float   matrix[16] =
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
        s_render->constantBuffers[ii] = gfxCreateConstantBuffer(graphics, kConstantBufferSize, matrix);
    }
}
void renderShutdown(void)
{
    for(int ii=0; ii<s_render->numVertexShaders; ++ii)
        gfxDestroyVertexShader(s_render->vertexShaders[ii]);
        
    for(int ii=0; ii<s_render->numPixelShaders; ++ii)
        gfxDestroyPixelShader(s_render->pixelShaders[ii]);
        
    for(int ii=0; ii<s_render->numMaterials; ++ii)
        gfxDestroyMaterial(s_render->materials[ii]);
        
    for(int ii=0; ii<s_render->numMeshes; ++ii)
        gfxDestroyMesh(s_render->meshes[ii]);
        
    for(int ii=0; ii<s_render->numTextures; ++ii)
        gfxDestroyTexture(s_render->textures[ii]);
    
    free(s_render);
    s_render = NULL;
}

vertex_shader_id_t renderCreateVertexShader(const char* filename)
{
    assert(s_render->numVertexShaders < kMaxMaterials);
    vertex_shader_id_t shaderId = s_render->numVertexShaders++;
    s_render->vertexShaders[shaderId] = gfxCreateVertexShader(s_render->graphics, filename);
    return shaderId;
}

pixel_shader_id_t renderCreatePixelShader(const char* filename)
{
    assert(s_render->numPixelShaders < kMaxMaterials);
    pixel_shader_id_t shaderId = s_render->numPixelShaders++;
    s_render->pixelShaders[shaderId] = gfxCreatePixelShader(s_render->graphics, filename);
    return shaderId;
}

material_id_t renderCreateMaterial(vertex_shader_id_t vertexShader, pixel_shader_id_t pixelShader)
{
    assert(s_render->numMaterials < kMaxMaterials);
    material_id_t materialId = s_render->numMaterials++;
    s_render->materials[materialId] = gfxCreateMaterial(s_render->graphics, s_render->vertexShaders[vertexShader], s_render->pixelShaders[pixelShader]);
    gfxBindConstantBufferToIndex(s_render->graphics, s_render->materials[materialId], "cbuffer0", 0);
    gfxBindConstantBufferToIndex(s_render->graphics, s_render->materials[materialId], "cbuffer1", 1);
    gfxBindConstantBufferToIndex(s_render->graphics, s_render->materials[materialId], "cbuffer2", 1);
    gfxBindConstantBufferToIndex(s_render->graphics, s_render->materials[materialId], "cbuffer3", 3);
    return materialId;
}

mesh_id_t renderCreateMesh( vertex_shader_id_t vertexShader,
                            vertex_format_e format,
                            uint32_t vertexCount,
                            uint32_t indexCount,
                            size_t vertexSize,
                            size_t indexSize,
                            const void* vertices,
                            const void* indices)
{
    assert(s_render->numMeshes < kMaxMeshes);
    mesh_id_t meshId = s_render->numMeshes++;
    s_render->meshes[meshId] = gfxCreateMesh(   s_render->graphics, 
                                                s_render->vertexShaders[vertexShader], 
                                                kVertexFormats[format], 
                                                vertexCount, indexCount, 
                                                vertexSize, indexSize, 
                                                vertices, indices);
    return meshId;
}

texture_id_t renderCreateTexture(const char* filename)
{
    assert(s_render->numTextures < kMaxTextures);
    texture_id_t textureId = s_render->numTextures++;
    s_render->textures[textureId] = gfxCreateTexture(s_render->graphics, filename);
    return textureId;
}

void renderSubmitDraw(  const float* viewProj,
                        material_id_t material, 
                        texture_id_t texture, 
                        const float* worldMatrix,
                        mesh_id_t mesh)
{
    assert(s_render->numRenderCommands < kMaxCommands);
    render_command_t& command = s_render->renderCommands[s_render->numRenderCommands++];
    command.texture = texture;
    command.material = material;
    command.mesh = mesh;
    memcpy(command.worldMatrix, worldMatrix, sizeof(float)*16);
    memcpy(command.viewProj, viewProj, sizeof(float)*16);
}

void renderFrame(void)
{
    graphics_t* graphics = s_render->graphics;
    
    gfxClear(graphics);
    for(int ii=0; ii<s_render->numRenderCommands; ++ii)
    {
        const render_command_t& command = s_render->renderCommands[ii];
        gfxSetMaterial(graphics, s_render->materials[command.material]);
        
    gfxBindConstantBufferToIndex(graphics, s_render->materials[command.material], "cbuffer0", 0);
    gfxBindConstantBufferToIndex(graphics, s_render->materials[command.material], "cbuffer1", 1);
    gfxBindConstantBufferToIndex(graphics, s_render->materials[command.material], "cbuffer2", 1);
    gfxBindConstantBufferToIndex(graphics, s_render->materials[command.material], "cbuffer3", 3);
        gfxUpdateConstantBuffer(graphics, s_render->constantBuffers[0], kConstantBufferSize, command.viewProj);
        gfxUpdateConstantBuffer(graphics, s_render->constantBuffers[1], kConstantBufferSize, command.worldMatrix);
        gfxSetVSConstantBuffer(graphics, s_render->constantBuffers[0], 0);
        gfxSetVSConstantBuffer(graphics, s_render->constantBuffers[1], 1);
        gfxSetTexture(graphics, s_render->textures[command.texture]);
        gfxDrawMesh(graphics, s_render->meshes[command.mesh]);
    }
    s_render->numRenderCommands = 0;
    
    gfxPresent(graphics);
}
