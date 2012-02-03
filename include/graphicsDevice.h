/*
 * graphicsDevice.h
 * RiotLib
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_graphicsDevice_h__
#define __RiotLib_graphicsDevice_h__

/* C headers */
#include <stdint.h>
#include <stddef.h>
/* C++ headers */
/* External headers */
/* Internal headers */

/*******************************************************************\
External Constants And types
\*******************************************************************/
struct graphics_t;
struct mesh_t;
struct vertex_shader_t;
struct pixel_shader_t;
struct material_t;
struct texture_t;
struct constant_buffer_t;

namespace ShaderInputSlot
{
    enum Enum
    {
        kSlot0,
        kSlot1,
        kSlot2,
        kSlot3,
        kSlot4,
        kSlot5,
        kSlot6,
        kSlot7,
        kSlot8,
        kSlot9,
        kSlot10,
        kSlot11,
        kSlot12,
        kSlot13,
        kSlot14,
        kSlot15,

        kPosition = kSlot0,
        kNormal,
        kColor,
        kTexCoord0,

        kGfxShaderInputNull = -1
    };
}

struct vertex_element_desc_t
{
    ShaderInputSlot::Enum slot;
    uint32_t count;
};

namespace GraphicsAPI
{
    enum Enum
    {
        kDirectX,
        kOpenGL,
        kNull
    };
}

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

class GraphicsDevice
{
private:
    GraphicsDevice();
    GraphicsDevice(const GraphicsDevice& _device);
    const GraphicsDevice& operator=(const GraphicsDevice& _device);
    virtual ~GraphicsDevice() {}

public:
    /* Graphics device managment */
    virtual void Create(void* window)       = 0;
    virtual void Destroy(void)              = 0;
    virtual GraphicsAPI::Enum GetAPI(void)  = 0;

    /* Device contols */
    virtual void Clear(void) = 0;
    virtual void Present(void) = 0;
    virtual void Resize(int width, int height) = 0;
    
    /* Pipeline management */
    virtual void SetClearColor(float r, float g, float b, float a, float depth) = 0;
    virtual void SetDepthTest(int test, int write) = 0;
    virtual void SetAlphaTest(int enable) = 0;

    /* Object creation */
    virtual vertex_shader_t* CreateVertexShader(const char* filename) = 0;
    virtual pixel_shader_t* CreatePixelShader(const char* filename) = 0;
    virtual material_t* CreateMaterial(vertex_shader_t* vertexShader, pixel_shader_t* pixelShader) = 0;
    virtual mesh_t* CreateMesh( vertex_shader_t* vertexShader,
                                const vertex_element_desc_t* layout,
                                uint32_t vertexCount,
                                uint32_t indexCount,
                                size_t vertexSize,
                                size_t indexSize,
                                const void* vertices,
                                const void* indices) = 0;
    virtual texture_t* CreateTexture(const char* filename) = 0;
    virtual constant_buffer_t* CreateConstantBuffer(size_t size, const void* data) = 0;

    /* object controls */
    virtual void UpdateConstantBuffer(constant_buffer_t* buffer, size_t size, const void* data) = 0;
    virtual void BindConstantBufferToIndex(material_t* material, const char* bufferName, int index) = 0;

    /* Object destruction */
    virtual void DestroyVertexShader(vertex_shader_t* shader) = 0;
    virtual void DestroyPixelShader(pixel_shader_t* shader) = 0;
    virtual void DestroyMaterial(material_t* material) = 0;
    virtual void DestroyMesh(mesh_t* mesh) = 0;
    virtual void DestroyTexture(texture_t* texture) = 0;
    virtual void DestroyConstantBuffer(constant_buffer_t* buffer) = 0;

    /* Render controls */
    virtual void SetMaterial(material_t* material) = 0;
    virtual void SetTexture(texture_t* texture) = 0;
    virtual void SetVSConstantBuffer(constant_buffer_t* buffer, uint32_t index) = 0;
    virtual void SetPSConstantBuffer(constant_buffer_t* buffer, uint32_t index) = 0;
    virtual void DrawMesh(mesh_t* mesh) = 0;
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
