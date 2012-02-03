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
#include <stddef.h>
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

typedef enum vertex_format_e
{
    kVtxFmtPosTex,
    kVtxFmtPosNormTex,

    kMAX_VERTEX_FORMATS
} vertex_format_e;

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void renderInit(void* window);
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

void renderSetViewProj(int index, const float* matrix);
void renderResize(int width, int height);

void renderSubmitDraw(  int view,
                        material_id_t material, 
                        texture_id_t texture, 
                        const float* worldMatrix,
                        mesh_id_t mesh);
void renderFrame(void);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif /* include guard */
