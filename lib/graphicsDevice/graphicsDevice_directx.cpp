/*
 * graphicsDevice_directx.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/3/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "graphicsDevice_directx.h"

/* C headers */
#include <stdio.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"
#include "file.h"

#define malloc_and_zero(pointer, type) pointer = (type)malloc(sizeof(*pointer)); memset(pointer, 0, sizeof(*pointer))

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
ID3DBlob* CompileShader(const char* filename, const char* profile)
{
    char        buffer[1024*8]  = {0};
    ID3DBlob*   errorBlob       = NULL;
    ID3DBlob*   shaderBlob      = NULL;
    const char* errorString     = NULL;
    uint32_t    compileFlags    = 0;
    HRESULT     hr              = S_OK;
    int         messageBoxResult= 0;
    size_t      bytesRead       = 0;

    memset(buffer, 0, sizeof(buffer));

#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
#endif

    do
    {
        File shaderFile(filename, "r");
        bytesRead = shaderFile.Read(buffer, sizeof(buffer[0]), sizeof(buffer));
        buffer[bytesRead] = '\0';
        shaderFile.Close();
        /* compile the shader */
        hr = D3DX11CompileFromMemory(   buffer,         /* Shader code */
                                        sizeof(buffer), /* Shader length */
                                        filename,       /* Filename */
                                        NULL,           /* Array of macro definitions */
                                        NULL,           /* #include interface */
                                        "main",         /* Function name */
                                        profile,        /* Shader profile */
                                        compileFlags,   /* Compile flags */
                                        0,              /* Not used for shaders, only effects */
                                        NULL,           /* Thread pump */
                                        &shaderBlob,    /* Compiled code */
                                        &errorBlob,     /* Errors */
                                        NULL );         /* HRESULT */
        if(FAILED(hr))
        {
            errorString = (const char*)errorBlob->GetBufferPointer();
            messageBoxResult = MessageBox(NULL, errorString, "Shader Compile Error", MB_RETRYCANCEL);
            SAFE_RELEASE(errorBlob);
        }
        else
            break;
    } while(messageBoxResult == IDRETRY);

    SAFE_RELEASE(errorBlob);
    return shaderBlob;
}
const char* GetSemanticNameFromIndex(ShaderInputSlot::Enum slot)
{
    switch(slot)
    {
    case ShaderInputSlot::kPosition:  return "POSITION";
    case ShaderInputSlot::kNormal:    return "NORMAL";
    case ShaderInputSlot::kColor:     return "COLOR";
    case ShaderInputSlot::kTexCoord0: return "TEXCOORD";
    default:
        assert(0);
    };
    return NULL;
}

} // anonymous namespace
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
void GraphicsDeviceDirectX::Create(void* window)
{
    DXGI_SWAP_CHAIN_DESC    swapChainDesc   = {0};
    char                    vsProfile[20]   = {0};
    char                    psProfile[20]   = {0};
    RECT                    clientRect      = {0};
    D3D11_RASTERIZER_DESC   rasterDesc;
    D3D11_SAMPLER_DESC      samplerDesc;
    D3D11_DEPTH_STENCIL_DESC    depthDesc   = {0};
    D3D11_BLEND_DESC        blendDesc       = {0};
    HWND                    hwnd            = (HWND)window;
    D3D_FEATURE_LEVEL       featureLevel    = D3D_FEATURE_LEVEL_9_1;
    HRESULT                 hr              = S_OK;
    uint32_t                width           = 0;
    uint32_t                height          = 0;
    uint32_t                creationFlags   = 0;

    ID3D11Device*           dxDevice      = NULL;
    ID3D11DeviceContext*    dxContext     = NULL;
    IDXGISwapChain*         dxSwapChain   = NULL;

    GetClientRect(hwnd, &clientRect);
    width = clientRect.right - clientRect.left;
    height = clientRect.bottom - clientRect.top;

    /*
     * Zero out self
     */
    memset( (char*)this + sizeof(void*) , 0, sizeof(*this)-sizeof(void*));

    /*
     * Create swap chain description
     */
    swapChainDesc.BufferCount          = 2;         // buffer count
    swapChainDesc.BufferDesc.Width     = width;    // width of the window
    swapChainDesc.BufferDesc.Height    = height;   // height of the window
    swapChainDesc.BufferDesc.Format    = DXGI_FORMAT_R8G8B8A8_UNORM;   // Back buffer format
    swapChainDesc.BufferDesc.RefreshRate.Numerator     = 60;   // Refresh rate
    swapChainDesc.BufferDesc.RefreshRate.Denominator   = 1;    // Refresh rate
    swapChainDesc.BufferUsage          = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Its a render target!
    swapChainDesc.OutputWindow         = hwnd;       // Window to attach to
    swapChainDesc.SampleDesc.Count     = 1;   // AA count
    swapChainDesc.SampleDesc.Quality   = 0; // AA quality
    swapChainDesc.Windowed             = 1;  // Windowed!! Yay!
    swapChainDesc.Flags                = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // TODO: Fix the fullscreen 'ding'.
                                                                                    // DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH isn't what causes it...
    /*
     * Create DX device
     */
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    hr = D3D11CreateDeviceAndSwapChain(    
        0,                        // physical adapter to try
        D3D_DRIVER_TYPE_HARDWARE, // TODO: Add support for software/warp
        0,                        // TODO: Add support for software/warp
        creationFlags,            // Creation flags
        NULL,                     // Feature levels to try
        0,                        // Number of feature levels
        D3D11_SDK_VERSION,        // The SDK version
        &swapChainDesc,           // The Swap chain description
        &dxSwapChain,             // The swap chain
        &dxDevice,                // the device
        &featureLevel,            // The feature level being used
        &dxContext );             // The context!
    assert(SUCCEEDED(hr));
    switch(featureLevel)
    {
    case D3D_FEATURE_LEVEL_11_0:
        strcpy_s(vsProfile, sizeof(vsProfile), "vs_5_0");
        strcpy_s(psProfile, sizeof(psProfile), "ps_5_0");
        break;
    case D3D_FEATURE_LEVEL_10_1:
        strcpy_s(vsProfile, sizeof(vsProfile), "vs_4_1");
        strcpy_s(psProfile, sizeof(psProfile), "ps_4_1");
        break;
    case D3D_FEATURE_LEVEL_10_0:
        strcpy_s(vsProfile, sizeof(vsProfile), "vs_4_0");
        strcpy_s(psProfile, sizeof(psProfile), "ps_4_0");
        break;
    case D3D_FEATURE_LEVEL_9_3:
        strcpy_s(vsProfile, sizeof(vsProfile), "vs_4_0_level_9_3");
        strcpy_s(psProfile, sizeof(psProfile), "ps_4_0_level_9_3");
        break;
    default:
        assert(0);
    };

    /*
     * Create the device
     */
    _context     = dxContext;
    _device      = dxDevice;
    _swapChain   = dxSwapChain;
    strcpy_s(_vsProfile, sizeof(_vsProfile), vsProfile);
    strcpy_s(_psProfile, sizeof(_psProfile), psProfile);

    /* Resize to create buffers */
    Resize(width, height);

    /*
     * Perform other initialization
     */

    /* rasterizer */
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.DepthClipEnable = TRUE;
    hr = _device->CreateRasterizerState(&rasterDesc, &_solidRasterState);
    assert(SUCCEEDED(hr));
    
    _context->RSSetState(_solidRasterState);
    _context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    /* samplers */
    memset(&samplerDesc, 0, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = _device->CreateSamplerState(&samplerDesc, &_linearSamplerState);
    assert(SUCCEEDED(hr));
    _context->PSSetSamplers(0, 1, &_linearSamplerState);

    /* Depth states */
    depthDesc.DepthEnable = 1;
    depthDesc.StencilEnable = 0;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    hr = _device->CreateDepthStencilState(&depthDesc, &_depthWriteTest);
    assert(SUCCEEDED(hr));
    
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr = _device->CreateDepthStencilState(&depthDesc, &_depthNoWriteTest);
    assert(SUCCEEDED(hr));
    
    depthDesc.DepthEnable = 0;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    hr = _device->CreateDepthStencilState(&depthDesc, &_depthWriteNoTest);
    assert(SUCCEEDED(hr));
    
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr = _device->CreateDepthStencilState(&depthDesc, &_depthNoWriteNoTest);
    assert(SUCCEEDED(hr));

    /* blend states */
    blendDesc.AlphaToCoverageEnable         = FALSE;
    blendDesc.IndependentBlendEnable        = FALSE;
    blendDesc.RenderTarget[0].BlendEnable   = TRUE;
    blendDesc.RenderTarget[0].SrcBlend      = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend     = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp       = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha= D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha  = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = _device->CreateBlendState(&blendDesc, &_blendAlpha);
    assert(SUCCEEDED(hr));

    blendDesc.RenderTarget[0].BlendEnable   = FALSE;
    blendDesc.RenderTarget[0].SrcBlend      = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend     = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOp       = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha  = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = _device->CreateBlendState(&blendDesc, &_blendNoAlpha);
    assert(SUCCEEDED(hr));
}
void GraphicsDeviceDirectX::Destroy(void)
{
    _swapChain->SetFullscreenState(0, NULL);
    SAFE_RELEASE(_blendAlpha);
    SAFE_RELEASE(_blendNoAlpha);

    SAFE_RELEASE(_depthWriteTest);
    SAFE_RELEASE(_depthNoWriteTest);
    SAFE_RELEASE(_depthWriteNoTest);
    SAFE_RELEASE(_depthNoWriteNoTest);

    SAFE_RELEASE(_linearSamplerState);
    SAFE_RELEASE(_solidRasterState);

    SAFE_RELEASE(_renderTargetView);
    SAFE_RELEASE(_depthStencilView);
    SAFE_RELEASE(_depthStencilResource);
    SAFE_RELEASE(_swapChain);
    SAFE_RELEASE(_context);
    SAFE_RELEASE(_device);

    delete this;
}
GraphicsAPI::Enum GraphicsDeviceDirectX::GetAPI(void)
{
    return GraphicsAPI::kDirectX;
}

/* Device contols */
void GraphicsDeviceDirectX::Clear(void)
{
    _context->ClearRenderTargetView(_renderTargetView, _clearColor);
    _context->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, _depthClear, 0);
}
void GraphicsDeviceDirectX::Present(void)
{
    HRESULT hr = S_OK;    
    /* TODO: Support occluded present test */
    hr = _swapChain->Present(0, 0); 
}
void GraphicsDeviceDirectX::Resize(int width, int height)
{
    HRESULT                         hr = S_OK;
    ID3D11Texture2D*                backBuffer = NULL;
    D3D11_TEXTURE2D_DESC            depthTextureDesc;
    D3D11_VIEWPORT                  viewportDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC   depthStencilDesc;

    if(_device == NULL)
        return;

    /* Release existing */
    _context->OMSetRenderTargets(0, NULL, NULL );
    SAFE_RELEASE(_renderTargetView);
    SAFE_RELEASE(_depthStencilView);
    SAFE_RELEASE(_depthStencilResource);

    _swapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    /* Create new render target */
    hr = _swapChain->GetBuffer(0, __uuidof(backBuffer), (void**)&backBuffer);
    assert(SUCCEEDED(hr));
    hr = _device->CreateRenderTargetView((ID3D11Resource*)backBuffer, NULL, &_renderTargetView);
    assert(SUCCEEDED(hr));
    SAFE_RELEASE(backBuffer);

    /* Depth stencil texture */
    ZeroMemory(&depthTextureDesc, sizeof(depthTextureDesc));
    depthTextureDesc.Width              = width;
    depthTextureDesc.Height             = height;
    depthTextureDesc.MipLevels          = 1;
    depthTextureDesc.ArraySize          = 1;
    depthTextureDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthTextureDesc.SampleDesc.Count   = 1;
    depthTextureDesc.SampleDesc.Quality = 0;
    depthTextureDesc.Usage              = D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    depthTextureDesc.CPUAccessFlags     = 0;
    depthTextureDesc.MiscFlags          = 0;
    hr = _device->CreateTexture2D(&depthTextureDesc, NULL, &_depthStencilResource);
    assert(SUCCEEDED(hr));

    /* Depth stencil view */
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
    depthStencilDesc.Format = depthTextureDesc.Format;
    depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilDesc.Texture2D.MipSlice = 0;
    depthStencilDesc.Flags = 0;
    hr = _device->CreateDepthStencilView((ID3D11Resource*)_depthStencilResource, &depthStencilDesc, &_depthStencilView);
    assert(SUCCEEDED(hr));

    /* Setup viewport */
    viewportDesc.Width    = (float)width;
    viewportDesc.Height   = (float)height;
    viewportDesc.MinDepth = 0.0f;
    viewportDesc.MaxDepth = 1.0f;
    viewportDesc.TopLeftX = 0;
    viewportDesc.TopLeftY = 0;
    _context->RSSetViewports(1, &viewportDesc);

    /* Restore render targets */
    _context->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView );
}
    
/* Pipeline management */
void GraphicsDeviceDirectX::SetClearColor(float r, float g, float b, float a, float depth)
{
    _clearColor[0] = r;
    _clearColor[1] = g;
    _clearColor[2] = b;
    _clearColor[3] = a;
    _depthClear = depth;
}
void GraphicsDeviceDirectX::SetDepthTest(int test, int write)
{
    if(test)
    {
        if(write)
            _context->OMSetDepthStencilState(_depthWriteTest, 0);
        else
            _context->OMSetDepthStencilState(_depthNoWriteTest, 0);
    }
    else
    {
        if(write)
            _context->OMSetDepthStencilState(_depthWriteNoTest, 0);
        else
            _context->OMSetDepthStencilState(_depthNoWriteNoTest, 0);
    }
}
void GraphicsDeviceDirectX::SetAlphaTest(int enable)
{
    float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    if(enable)
        _context->OMSetBlendState(_blendAlpha, blendFactor, 0xFFFFFFFF);
    else
        _context->OMSetBlendState(_blendNoAlpha, blendFactor, 0xFFFFFFFF);
}

/* Object creation */
vertex_shader_t* GraphicsDeviceDirectX::CreateVertexShader(const char* filename)
{
    vertex_shader_t*    shader              = NULL;
    ID3D11VertexShader* vertexShader        = NULL;
    ID3DBlob*           vertexShaderBlob    = NULL;
    void*               shaderCode          = NULL;
    size_t              shaderSize          = 0;
    HRESULT             hr                  = S_OK;

    vertexShaderBlob = CompileShader(filename, _vsProfile);
    assert(vertexShaderBlob);
    shaderCode = vertexShaderBlob->GetBufferPointer();
    shaderSize = vertexShaderBlob->GetBufferSize();
    hr = _device->CreateVertexShader(shaderCode, shaderSize, NULL, &vertexShader);
    assert(SUCCEEDED(hr));

    /* Create new shader */
    malloc_and_zero(shader, vertex_shader_t*);
    shader->shader      = vertexShader;
    shader->shaderBlob  = vertexShaderBlob;

    return shader;
}
pixel_shader_t* GraphicsDeviceDirectX::CreatePixelShader(const char* filename)
{
    pixel_shader_t*     shader          = NULL;
    ID3D11PixelShader*  pixelShader     = NULL;
    ID3DBlob*           pixelShaderBlob = NULL;
    void*               shaderCode      = NULL;
    size_t              shaderSize      = 0;
    HRESULT             hr              = S_OK;

    pixelShaderBlob = CompileShader(filename, _psProfile);
    assert(pixelShaderBlob);
    shaderCode = pixelShaderBlob->GetBufferPointer();
    shaderSize = pixelShaderBlob->GetBufferSize();
    hr = _device->CreatePixelShader(shaderCode, shaderSize, NULL, &pixelShader);
    assert(SUCCEEDED(hr));

    /* Create new shader */
    malloc_and_zero(shader, pixel_shader_t*);
    shader->shader  = pixelShader;

    return shader;
}
material_t* GraphicsDeviceDirectX::CreateMaterial(vertex_shader_t* vertexShader, pixel_shader_t* pixelShader)
{
    material_t* material = NULL;

    /* Allocate material */
    malloc_and_zero(material, material_t*);    
    material->vertexShader  = vertexShader->shader;
    material->pixelShader   = pixelShader->shader;
    material->vertexShader->AddRef();
    material->pixelShader->AddRef();
      
    return material;
}
mesh_t* GraphicsDeviceDirectX::CreateMesh( vertex_shader_t* vertexShader,
                                        const vertex_element_desc_t* layout,
                                        uint32_t vertexCount,
                                        uint32_t indexCount,
                                        size_t vertexSize,
                                        size_t indexSize,
                                        const void* vertices,
                                        const void* indices)
{
    D3D11_INPUT_ELEMENT_DESC    inputDesc[16] = {0};
    D3D11_BUFFER_DESC           bufferDesc  = {0};
    D3D11_SUBRESOURCE_DATA      initData    = {0};
    ID3D11InputLayout*          inputLayout = NULL;
    ID3D11Buffer*               vertexBuffer= NULL;
    ID3D11Buffer*               indexBuffer = NULL;
    mesh_t*                     mesh        = NULL;
    HRESULT                     hr          = S_OK;
    int                         layoutCount = 0;
    DXGI_FORMAT                 layoutFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

    /* vertex buffer */
    bufferDesc.Usage            = D3D11_USAGE_IMMUTABLE;
    bufferDesc.ByteWidth        = (UINT)(vertexSize*vertexCount);
    bufferDesc.BindFlags        = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags   = 0;
    initData.pSysMem            = vertices;
    hr = _device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
    assert(SUCCEEDED(hr));
    
    /* index buffer */
    bufferDesc.ByteWidth        = (UINT)(indexSize*indexCount);
    bufferDesc.BindFlags        = D3D11_BIND_INDEX_BUFFER;
    initData.pSysMem            = indices;
    hr = _device->CreateBuffer(&bufferDesc, &initData, &indexBuffer);
    assert(SUCCEEDED(hr));

    /* input layout */
    while(layout && layout->slot != ShaderInputSlot::kNull)
    {
        switch(layout->count)
        {
        case 4: layoutFormat = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
        case 3: layoutFormat = DXGI_FORMAT_R32G32B32_FLOAT; break;
        case 2: layoutFormat = DXGI_FORMAT_R32G32_FLOAT; break;
        case 0: assert(0); break;
        default:
            assert(0);
        }
        inputDesc[layoutCount].SemanticName         = GetSemanticNameFromIndex(layout->slot);
        inputDesc[layoutCount].SemanticIndex        = 0;
        inputDesc[layoutCount].Format               = layoutFormat;
        inputDesc[layoutCount].AlignedByteOffset    = (layoutCount == 0) ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
        inputDesc[layoutCount].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
        inputDesc[layoutCount].InstanceDataStepRate = 0;
        ++layoutCount;
        ++layout;
    }
    hr = _device->CreateInputLayout(
                                        inputDesc, 
                                        layoutCount, 
                                        vertexShader->shaderBlob->GetBufferPointer(), 
                                        vertexShader->shaderBlob->GetBufferSize(), 
                                        &inputLayout);
    assert(SUCCEEDED(hr));

    /* create mesh object */
    malloc_and_zero(mesh, mesh_t*);
    mesh->vertexBuffer = vertexBuffer;
    mesh->indexBuffer  = indexBuffer;
    mesh->inputLayout  = inputLayout;
    mesh->indexCount   = (uint32_t)indexCount;
    mesh->vertexStride = (uint32_t)vertexSize;
    mesh->indexStride  = (uint32_t)indexSize;

    return mesh;
}
texture_t* GraphicsDeviceDirectX::CreateTexture(const char* filename)
{
    texture_t*                  texture     = NULL;
    ID3D11ShaderResourceView*   resourceView= NULL;
    HRESULT                     hr          = S_OK;

    hr = D3DX11CreateShaderResourceViewFromFile(_device, filename, NULL, NULL, &resourceView, NULL); 
    assert(SUCCEEDED(hr));

    /* allocate texture */
    malloc_and_zero(texture, texture_t*);
    texture->resourceView = resourceView;

    return texture;
}
constant_buffer_t* GraphicsDeviceDirectX::CreateConstantBuffer(size_t size, const void* data)
{
    constant_buffer_t*      constantBuffer  = NULL;
    ID3D11Buffer*           buffer          = NULL;
    D3D11_BUFFER_DESC       bufferDesc      = { 0 };
    D3D11_SUBRESOURCE_DATA  initData        = { 0 };
    HRESULT                 hr              = S_OK;

    bufferDesc.Usage            = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth        = (UINT)size;
    bufferDesc.BindFlags        = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags   = D3D11_CPU_ACCESS_WRITE;
    initData.pSysMem            = data;

    hr = _device->CreateBuffer((data) ? &bufferDesc : NULL, &initData, &buffer);
    assert(SUCCEEDED(hr));

    /* allocate buffer */
    malloc_and_zero(constantBuffer, constant_buffer_t*);
    constantBuffer->buffer = buffer;

    return constantBuffer;
}

/* object controls */
void GraphicsDeviceDirectX::UpdateConstantBuffer(constant_buffer_t* buffer, size_t size, const void* data)
{
    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    _context->Map((ID3D11Resource*)buffer->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
    memcpy(mappedBuffer.pData, data, size);
    _context->Unmap((ID3D11Resource*)buffer->buffer, 0);
}
void GraphicsDeviceDirectX::BindConstantBufferToIndex(material_t* material, const char* bufferName, int index)
{
    UNUSED_PARAMETER(material);
    UNUSED_PARAMETER(bufferName);
    UNUSED_PARAMETER(index);
}

/* Object destruction */
void GraphicsDeviceDirectX::DestroyVertexShader(vertex_shader_t* shader)
{
    SAFE_RELEASE(shader->shaderBlob);
    SAFE_RELEASE(shader->shader);
    free(shader);
}
void GraphicsDeviceDirectX::DestroyPixelShader(pixel_shader_t* shader)
{
    SAFE_RELEASE(shader->shader);
    free(shader);
}
void GraphicsDeviceDirectX::DestroyMaterial(material_t* material)
{
    SAFE_RELEASE(material->pixelShader);
    SAFE_RELEASE(material->vertexShader);
    free(material);
}
void GraphicsDeviceDirectX::DestroyMesh(mesh_t* mesh)
{
    SAFE_RELEASE(mesh->vertexBuffer);
    SAFE_RELEASE(mesh->indexBuffer);
    SAFE_RELEASE(mesh->inputLayout);
    free(mesh);
}
void GraphicsDeviceDirectX::DestroyTexture(texture_t* texture)
{
    SAFE_RELEASE(texture->resourceView);
    free(texture);
}
void GraphicsDeviceDirectX::DestroyConstantBuffer(constant_buffer_t* buffer)
{
    SAFE_RELEASE(buffer->buffer);
    free(buffer);
}

/* Render controls */
void GraphicsDeviceDirectX::SetMaterial(material_t* material)
{
    _context->VSSetShader(material->vertexShader, NULL, 0);
    _context->PSSetShader(material->pixelShader, NULL, 0);
}
void GraphicsDeviceDirectX::SetTexture(texture_t* texture)
{
    _context->PSSetShaderResources(0, 1, &texture->resourceView);
}
void GraphicsDeviceDirectX::SetVSConstantBuffer(constant_buffer_t* buffer, uint32_t index)
{
    _context->VSSetConstantBuffers(index, 1, &buffer->buffer);
}
void GraphicsDeviceDirectX::SetPSConstantBuffer(constant_buffer_t* buffer, uint32_t index)
{
    _context->PSSetConstantBuffers(index, 1, &buffer->buffer);
}
void GraphicsDeviceDirectX::DrawMesh(mesh_t* mesh)
{
    UINT    offset = 0;
    DXGI_FORMAT indexFormat = (mesh->indexStride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    _context->IASetInputLayout(mesh->inputLayout);
    _context->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &mesh->vertexStride, &offset);
    _context->IASetIndexBuffer(mesh->indexBuffer, indexFormat, 0);
    _context->DrawIndexed(mesh->indexCount, 0, 0);
}
