/*
 * graphicsDevice_null.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "graphicsDevice_null.h"

/* C headers */
#include <stdio.h>
/* C++ headers */
/* External headers */
/* Internal headers */

#define PRINT_FUNC printf("%s called\n", __FUNCTION__)

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
int meshIndex           = 0;
int vertexShaderIndex   = 0;
int pixelShaderIndex    = 0;
int materialIndex       = 0;
int textureIndex        = 0;
int constantBuferIndex  = 0;

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
void GraphicsDeviceNull::Create(void* window)
{
    PRINT_FUNC;
}
void GraphicsDeviceNull::Destroy(void)
{
    PRINT_FUNC;
    delete this;
}
GraphicsAPI::Enum GraphicsDeviceNull::GetAPI(void)
{
    return GraphicsAPI::kNull;
}

/* Device contols */
void GraphicsDeviceNull::Clear(void)
{
    PRINT_FUNC;
}
void GraphicsDeviceNull::Present(void)
{
    PRINT_FUNC;
}
void GraphicsDeviceNull::Resize(int width, int height)
{
    PRINT_FUNC;
}
    
/* Pipeline management */
void GraphicsDeviceNull::SetClearColor(float r, float g, float b, float a, float depth)
{
    PRINT_FUNC;
}
void GraphicsDeviceNull::SetDepthTest(int test, int write)
{
    PRINT_FUNC;
}
void GraphicsDeviceNull::SetAlphaTest(int enable)
{
    PRINT_FUNC;
}

/* Object creation */
vertex_shader_t* GraphicsDeviceNull::CreateVertexShader(const char* filename)
{
    printf("%s called with parameter: %s\n", __FUNCTION__, filename);
    return (vertex_shader_t*)++vertexShaderIndex;
}
pixel_shader_t* GraphicsDeviceNull::CreatePixelShader(const char* filename)
{
    printf("%s called with parameter: %s\n", __FUNCTION__, filename);
    return (pixel_shader_t*)++pixelShaderIndex;
}
material_t* GraphicsDeviceNull::CreateMaterial(vertex_shader_t* vertexShader, pixel_shader_t* pixelShader)
{
    printf("%s called with parameters: 0x%p, 0x%p\n", __FUNCTION__, vertexShader, pixelShader);
    return (material_t*)++materialIndex;
}
mesh_t* GraphicsDeviceNull::CreateMesh( vertex_shader_t* vertexShader,
                                        const vertex_element_desc_t* layout,
                                        uint32_t vertexCount,
                                        uint32_t indexCount,
                                        size_t vertexSize,
                                        size_t indexSize,
                                        const void* vertices,
                                        const void* indices)
{
    printf("%s called with parameters:\n", __FUNCTION__);
    printf("Vertex shader: %p\n", vertexShader);
    printf("Vertex layout:\n");
    while(layout && layout->slot != ShaderInputSlot::kNull)
    {
        printf("\tSlot: %d, Count: %d\n", layout->slot, layout->count);
        layout++;
    }
    printf("VertexCount:\t%d\n", vertexCount);
    printf("IndexCount:\t%d\n", indexCount);
    printf("VertexSize:\t%d\n", vertexSize);
    printf("IndexSize:\t%d\n", indexSize);
    printf("Vertices:\t%p\n", vertices);
    printf("indices:\t%p\n", indices);
    return (mesh_t*)++meshIndex;
}
texture_t* GraphicsDeviceNull::CreateTexture(const char* filename)
{
    printf("%s called with parameter: %s\n", __FUNCTION__, filename);
    return (texture_t*)++textureIndex;
}
constant_buffer_t* GraphicsDeviceNull::CreateConstantBuffer(size_t size, const void* data)
{
    printf("%s called with size: %d and data %p\n", __FUNCTION__, size, data);
    return (constant_buffer_t*)++constantBuferIndex;
}

/* object controls */
void GraphicsDeviceNull::UpdateConstantBuffer(constant_buffer_t* buffer, size_t size, const void* data)
{
    printf("%s called with buffer: %p  size: %d and data %p\n", __FUNCTION__, buffer, size, data);
}
void GraphicsDeviceNull::BindConstantBufferToIndex(material_t* material, const char* bufferName, int index)
{
    printf("%s called with bufferName: %s and index: %d\n", __FUNCTION__, bufferName, index);
}
void GraphicsDeviceNull::UpdateMeshData(mesh_t* mesh, const void* vertices)
{
    printf("Updating mesh %p with data %p\n", mesh, vertices);
}

/* Object destruction */
void GraphicsDeviceNull::DestroyVertexShader(vertex_shader_t* shader)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, shader);
}
void GraphicsDeviceNull::DestroyPixelShader(pixel_shader_t* shader)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, shader);
}
void GraphicsDeviceNull::DestroyMaterial(material_t* material)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, material);
}
void GraphicsDeviceNull::DestroyMesh(mesh_t* mesh)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, mesh);
}
void GraphicsDeviceNull::DestroyTexture(texture_t* texture)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, texture);
}
void GraphicsDeviceNull::DestroyConstantBuffer(constant_buffer_t* buffer)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, buffer);
}

/* Render controls */
void GraphicsDeviceNull::SetMaterial(material_t* material)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, material);
}
void GraphicsDeviceNull::SetTexture(texture_t* texture)
{
    printf("%s called with parameter: %p\n", __FUNCTION__, texture);
}
void GraphicsDeviceNull::SetVSConstantBuffer(constant_buffer_t* buffer, uint32_t index)
{
    printf("%s called with buffer: %p and index: %d\n", __FUNCTION__, buffer, index, index);
}
void GraphicsDeviceNull::SetPSConstantBuffer(constant_buffer_t* buffer, uint32_t index)
{
    printf("%s called with buffer: %p and index: %d\n", __FUNCTION__, buffer, index, index);
}
void GraphicsDeviceNull::DrawMesh(mesh_t* mesh)
{
    printf("%s called with mesh: %p\n", __FUNCTION__, mesh);
}
