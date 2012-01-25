/*
 * renderEngine.h
 * renderEngine
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __renderEngine_renderEngine_h__
#define __renderEngine_renderEngine_h__

/* C headers */
#include <stdint.h>
/* C++ headers */
/* External headers */
/* Internal headers */


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*******************************************************************\
External Constants And types
\*******************************************************************/
struct graphics_t;
typedef int mesh_id_t;
typedef int vertex_shader_id_t;
typedef int pixel_shader_id_t;
typedef int material_id_t;
typedef int texture_id_t;
typedef int constant_buffer_id_t;

typedef enum vertex_format_e
{
    kVtxFmtPosTex,
    kVtxFmtPosNormTex,

    kMAX_VERTEX_FORMATS,
} vertex_format_e;

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void renderInit(graphics_t* graphics);
void renderShutdown(void);

vertex_shader_id_t renderCreateVertexShader(const char* filename);
pixel_shader_id_t renderCreatePixelShader(const char* filename);
material_id_t renderCreateMaterial(vertex_shader_id_t vertexShader, pixel_shader_id_t pixelShader);
mesh_id_t renderCreateMesh( vertex_shader_id_t vertexShader,
                            vertex_format_e format,
                            uint32_t vertexCount,
                            uint32_t indexCount,
                            size_t vertexSize,
                            size_t indexSize,
                            const void* vertices,
                            const void* indices);
texture_id_t renderCreateTexture(const char* filename);
constant_buffer_id_t renderCreateConstantBuffer(size_t size, const void* data);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif /* include guard */
