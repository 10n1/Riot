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
};
struct render_t
{
    graphics_t* graphics;

    vertex_shader_t*    vertexShaders[kMaxMaterials];
    pixel_shader_t*     pixelShaders[kMaxMaterials];
    material_t*         materials[kMaxMaterials];
    mesh_t*             meshes[kMaxMeshes];
    texture_t*          textures[kMaxTextures];
    constant_buffer_t*  constantBuffers[1];
    
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

    /* Fill it out */
}
void renderShutdown(void)
{
}

vertex_shader_id_t renderCreateVertexShader(const char* filename)
{
    return 0;
}

pixel_shader_id_t renderCreatePixelShader(const char* filename)
{
    return 0;
}

material_id_t renderCreateMaterial(vertex_shader_id_t vertexShader, pixel_shader_id_t pixelShader)
{
    return 0;
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
    return 0;
}

texture_id_t renderCreateTexture(const char* filename)
{
    return 0;
}

constant_buffer_id_t renderCreateConstantBuffer(size_t size, const void* data)
{
    return 0;
}
