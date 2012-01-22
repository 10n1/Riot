/*
 * graphicsDevice.h
 * graphicsDevice
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __graphicsDevice_graphicsDevice_h__
#define __graphicsDevice_graphicsDevice_h__

/* C headers */
#include <stdint.h>
#include <stddef.h>
/* C++ headers */
/* External headers */
/* Internal headers */

#define GD_DIRECTX 1
#define GD_OPENGL  2

#ifndef GD_API
    #define GD_API GD_OPENGL
#endif

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*******************************************************************\
External Constants And types
\*******************************************************************/
typedef struct graphics_t           graphics_t;
typedef struct mesh_t               mesh_t;
typedef struct vertex_shader_t      vertex_shader_t;
typedef struct pixel_shader_t       pixel_shader_t;
typedef struct material_t           material_t;
typedef struct texture_t            texture_t;
typedef struct constant_buffer_t    constant_buffer_t;
typedef enum
{
    kGfxShaderInputSlot0,
    kGfxShaderInputSlot1,
    kGfxShaderInputSlot2,
    kGfxShaderInputSlot3,
    kGfxShaderInputSlot4,
    kGfxShaderInputSlot5,
    kGfxShaderInputSlot6,
    kGfxShaderInputSlot7,
    kGfxShaderInputSlot8,
    kGfxShaderInputSlot9,
    kGfxShaderInputSlot10,
    kGfxShaderInputSlot11,
    kGfxShaderInputSlot12,
    kGfxShaderInputSlot13,
    kGfxShaderInputSlot14,
    kGfxShaderInputSlot15,
    
    kGfxShaderInputPosition = kGfxShaderInputSlot0,
    kGfxShaderInputNormal,
    kGfxShaderInputColor,
    kGfxShaderInputTexCoord0,
    
    kShaderInputNull = -1
} shader_input_slot_e;

typedef struct vertex_element_desc_t
{
    shader_input_slot_e slot;
    uint32_t count;
} vertex_element_desc_t;

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
/* Graphics device managment */
graphics_t* gfxCreate(void* window);
void gfxDestroy(graphics_t* device);

/* Device contols */
void gfxClear(graphics_t* device);
void gfxPresent(graphics_t* device);
void gfxResize(graphics_t* device, int width, int height);

/* Pipeline management */
void gfxSetClearColor(graphics_t* device, float r, float g, float b, float a, float depth);
void gfxSetDepthTest(graphics_t* device, int test, int write);
void gfxSetAlphaTest(graphics_t* device, int enable);

/* Object creation */
vertex_shader_t* gfxCreateVertexShader(graphics_t* device, const char* filename);
pixel_shader_t* gfxCreatePixelShader(graphics_t* device, const char* filename);
material_t* gfxCreateMaterial(graphics_t* device, vertex_shader_t* vertexShader, pixel_shader_t* pixelShader);
mesh_t* gfxCreateMesh(graphics_t* device,
                     vertex_shader_t* vertexShader,
                     const vertex_element_desc_t* layout,
                     uint32_t vertexCount,
                     uint32_t indexCount,
                     size_t vertexSize,
                     size_t indexSize,
                     const void* vertices,
                     const void* indices);
texture_t* gfxCreateTexture(graphics_t* device, const char* filename);
constant_buffer_t* gfxCreateConstantBuffer(graphics_t* device, size_t size, const void* data);

/* object controls */
void gfxUpdateConstantBuffer(graphics_t* device, constant_buffer_t* buffer, size_t size, const void* data);
void gfxBindConstantBufferToIndex(graphics_t* device, material_t* material, const char* bufferName, int index);

/* Object destruction */
void gfxDestroyVertexShader(vertex_shader_t* shader);
void gfxDestroyPixelShader(pixel_shader_t* shader);
void gfxDestroyMaterial(material_t* material);
void gfxDestroyMesh(mesh_t* mesh);
void gfxDestroyTexture(texture_t* texture);
void gfxDestroyConstantBuffer(constant_buffer_t* buffer);

/* Render controls */
void gfxSetMaterial(graphics_t* device, material_t* material);
void gfxSetTexture(graphics_t* device, texture_t* texture);
void gfxSetVSConstantBuffer(graphics_t* device, constant_buffer_t* buffer, uint32_t index);
void gfxSetPSConstantBuffer(graphics_t* device, constant_buffer_t* buffer, uint32_t index);
void gfxDrawMesh(graphics_t* device, mesh_t* mesh);

#ifndef GD_API
    #define GD_API GD_OPENGL
#else
    #ifndef WIN32
        #undef GD_API
        #define GD_API GD_OPENGL
    #endif
#endif


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif /* include guard */
