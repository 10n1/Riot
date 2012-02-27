/*
 * graphicsDevice_null.h
 * RiotLib
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_graphicsDevice_null_h__
#define __RiotLib_graphicsDevice_null_h__

/* C headers */
#include <stdint.h>
#include <stddef.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "graphicsDevice.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

class GraphicsDeviceNull : public GraphicsDevice
{
public:
    /* Graphics device managment */
    void Create(void* window);
    void Destroy(void);
    GraphicsAPI::Enum GetAPI(void);

    /* Device contols */
    void Clear(void);
    void Present(void);
    void Resize(int width, int height);
    
    /* Pipeline management */
    void SetClearColor(float r, float g, float b, float a, float depth);
    void SetDepthTest(int test, int write);
    void SetAlphaTest(int enable);

    /* Object creation */
    vertex_shader_t* CreateVertexShader(const char* filename);
    pixel_shader_t* CreatePixelShader(const char* filename);
    material_t* CreateMaterial(vertex_shader_t* vertexShader, pixel_shader_t* pixelShader);
    mesh_t* CreateMesh( vertex_shader_t* vertexShader,
                                const vertex_element_desc_t* layout,
                                uint32_t vertexCount,
                                uint32_t indexCount,
                                size_t vertexSize,
                                size_t indexSize,
                                const void* vertices,
                                const void* indices);
    texture_t* CreateTexture(const char* filename);
    constant_buffer_t* CreateConstantBuffer(size_t size, const void* data);

    /* object controls */
    void UpdateConstantBuffer(constant_buffer_t* buffer, size_t size, const void* data);
    void BindConstantBufferToIndex(material_t* material, const char* bufferName, int index);
    void UpdateMeshData(mesh_t* mesh, const void* vertices);

    /* Object destruction */
    void DestroyVertexShader(vertex_shader_t* shader);
    void DestroyPixelShader(pixel_shader_t* shader);
    void DestroyMaterial(material_t* material);
    void DestroyMesh(mesh_t* mesh);
    void DestroyTexture(texture_t* texture);
    void DestroyConstantBuffer(constant_buffer_t* buffer);

    /* Render controls */
    void SetMaterial(material_t* material);
    void SetTexture(texture_t* texture);
    void SetVSConstantBuffer(constant_buffer_t* buffer, uint32_t index);
    void SetPSConstantBuffer(constant_buffer_t* buffer, uint32_t index);
    void DrawMesh(mesh_t* mesh);
};


#ifndef GFX_API
    #define GFX_API GFX_OPENGL
#else
    #ifndef WIN32
        #undef GFX_API
        #define GFX_API GFX_OPENGL
    #endif
#endif

#endif /* include guard */
