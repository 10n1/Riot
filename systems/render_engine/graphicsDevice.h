/*
 *  graphicsDevice.h
 *  graphicsDevice.lib
 *
 *  Created by Kyle Weicht on 1/10/12.
 *  Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __graphicsDevice_h__
#define __graphicsDevice_h__

#include <stdint.h>

//#include <c++headers>

//#include "external_headers.h"

//#include "internal_headers.h"

namespace GraphicsDevice
{

/*******************************************************************\
 External constants and types
\*******************************************************************/
enum shader_input_slot_e
{
    kShaderInputSlot0,
    kShaderInputSlot1,
    kShaderInputSlot2,
    kShaderInputSlot3,
    kShaderInputSlot4,
    kShaderInputSlot5,
    kShaderInputSlot6,
    kShaderInputSlot7,
    kShaderInputSlot8,
    kShaderInputSlot9,
    kShaderInputSlot10,
    kShaderInputSlot11,
    kShaderInputSlot12,
    kShaderInputSlot13,
    kShaderInputSlot14,
    kShaderInputSlot15,
    
    kShaderInputPosition = kShaderInputSlot0,
    kShaderInputNormal,
    kShaderInputColor,
    kShaderInputTexCoord0,
    
    kShaderInputNull = -1
};

union shader_t
{
    void*   pointerShader;
    int64_t intShader;
};
union buffer_t
{
    void*   pointerBuffer;
    int64_t intBuffer;
};
union vertex_format_t
{
    void*   pointerFormat;
    int64_t intFormat;
};
struct vertex_layout_t
{
    int slot;
    int count;
};
enum index_format_e
{
    kIndex16,
    kIndex32,
};
union texture_t
{
    void*   pointerTexture;
    int64_t intTexture;
};
struct mesh_t
{
    buffer_t    vertexBuffer;
    buffer_t    indexBuffer;
    vertex_format_t vertexLayout;
    index_format_e  indexFormat;
    int             indexCount;
};
typedef uint32_t program_t;

/*******************************************************************\
 Variables
\*******************************************************************/

/*******************************************************************\
 External functions
\*******************************************************************/

// Device functions
void Initialize(void* window);
void Shutdown(void);

// Pipeline control
void SetClearColor(float r, float g, float b, float a);
void SetClearDepth(float d);

// Frame controls
void Clear(void);
void Present(void);

// Render object creation
shader_t CreateVertexShader(const char* shaderSource);
shader_t CreatePixelShader(const char* shaderSource);
program_t CreateProgram(shader_t vertexShader, shader_t pixelShader);
buffer_t CreateVertexBuffer(size_t size, const void* data);
buffer_t CreateIndexBuffer(size_t size, const void* data);
vertex_format_t CreateVertexLayout(const vertex_layout_t* layout, int vertexStride);
texture_t CreateTexture(int width, int height, int bits, void* data);
mesh_t CreateMesh(  const vertex_layout_t* layout,
                    int indexCount, 
                    int vertexCount, 
                    size_t vertexStride, 
                    size_t indexSize, 
                    const void* vertices, 
                    const void* indices);

// Set functions
void SetVertexShader(shader_t shader);
void SetPixelShader(shader_t shader);
void SetProgram(program_t program);
void SetVertexLayout(vertex_format_t layout);
void BindVertexBuffer(buffer_t buffer);
void BindIndexBuffer(buffer_t buffer);

void UpdateTextureData(texture_t texture, int width, int height, int bits, void* data);

// Draw commands
void Draw(index_format_e indexFormat, int indexCount);
void DrawMesh(mesh_t mesh);

} // namespace Render

#endif /* include guard */
