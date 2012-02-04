/*
 * graphicsDevice_directx.h
 * RiotLib
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_graphicsDevice_directx_h__
#define __RiotLib_graphicsDevice_directx_h__

/* C headers */
#include <stdint.h>
#include <stddef.h>
/* C++ headers */
#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
/* External headers */
/* Internal headers */
#include "graphicsDevice.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
struct vertex_shader_t
{
    ID3D11VertexShader* shader;
    ID3DBlob*           shaderBlob;
};
struct pixel_shader_t
{
    ID3D11PixelShader* shader;
};
struct material_t
{
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader*  pixelShader;    
};
struct mesh_t
{
    ID3D11InputLayout*  inputLayout;
    ID3D11Buffer*       vertexBuffer;
    ID3D11Buffer*       indexBuffer;
    uint32_t            indexStride;
    uint32_t            vertexStride;
    uint32_t            indexCount;
};
struct texture_t
{
    ID3D11ShaderResourceView*   resourceView;
};
struct constant_buffer_t
{
    ID3D11Buffer*   buffer;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
class GraphicsDeviceDirectX : public GraphicsDevice
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

private:
    char                    _vsProfile[20];
    char                    _psProfile[20];
    ID3D11Device*           _device;
    ID3D11DeviceContext*    _context;
    IDXGISwapChain*         _swapChain;
    ID3D11RenderTargetView* _renderTargetView;
    ID3D11DepthStencilView* _depthStencilView;
    ID3D11Texture2D*        _depthStencilResource;
    ID3D11RasterizerState*  _solidRasterState;
    ID3D11SamplerState*     _linearSamplerState;

    ID3D11DepthStencilState*    _depthWriteTest;
    ID3D11DepthStencilState*    _depthNoWriteTest;
    ID3D11DepthStencilState*    _depthWriteNoTest;
    ID3D11DepthStencilState*    _depthNoWriteNoTest;

    ID3D11BlendState*   _blendAlpha;
    ID3D11BlendState*   _blendNoAlpha;

    float   _clearColor[4];
    float   _depthClear;
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
