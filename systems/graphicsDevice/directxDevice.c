/*
 * openglDevice.c
 * graphicsDevice
 *
 * Created by Kyle Weicht on 1/15/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "graphicsDevice.h"

#if (GD_API == GD_DIRECTX)

#define CINTERFACE
#define COBJMACROS

/* C headers */
#include <stdint.h>
#include <stdio.h>
#include <malloc.h>
#pragma warning(push)
#pragma warning(disable:4201)
#include <D3D11.h>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#pragma warning(pop)
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"

#define SAFE_RELEASE(x) if((x)) { x->lpVtbl->Release((x)); x = NULL; }
#define ADD_REF(x) x->lpVtbl->AddRef((x))

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
struct graphics_t
{
    char                    vsProfile[20];
    char                    psProfile[20];
    ID3D11Device*           device;
    ID3D11DeviceContext*    context;
    IDXGISwapChain*         swapChain;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11DepthStencilView* depthStencilView;
    ID3D11Texture2D*        depthStencilResource;
    ID3D11RasterizerState*  solidRasterState;
    ID3D11SamplerState*     linearSamplerState;

    ID3D11DepthStencilState*    depthWriteTest;
    ID3D11DepthStencilState*    depthNoWriteTest;
    ID3D11DepthStencilState*    depthWriteNoTest;
    ID3D11DepthStencilState*    depthNoWriteNoTest;

    ID3D11BlendState*   blendAlpha;
    ID3D11BlendState*   blendNoAlpha;

    float   clearColor[4];
    float   depthClear;
};

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
Internal variables
\*******************************************************************/

/*******************************************************************\
Internal functions
\*******************************************************************/
static ID3DBlob* CompileShader(const char* filename, const char* profile)
{
    char        buffer[1024*8]  = {0};
    ID3DBlob*   errorBlob       = NULL;
    ID3DBlob*   shaderBlob      = NULL;
    const char* errorString     = NULL;
    uint32_t    compileFlags    = 0;
    HRESULT     hr              = S_OK;
    int         messageBoxResult= 0;
    FILE*       shaderFile      = NULL;
    size_t      bytesRead       = 0;

    memset(buffer, 0, sizeof(buffer));

#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
#endif

    do
    {
        shaderFile = fopen(filename, "r");
        bytesRead = fread(buffer, sizeof(buffer[0]), sizeof(buffer), shaderFile);
        buffer[bytesRead] = '\0';
        fclose(shaderFile);
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
            errorString = (const char*)ID3D10Blob_GetBufferPointer(errorBlob);
            messageBoxResult = MessageBox(NULL, errorString, "Shader Compile Error", MB_RETRYCANCEL);
            SAFE_RELEASE(errorBlob);
        }
        else
            break;
    } while(messageBoxResult == IDRETRY);

    SAFE_RELEASE(errorBlob);
    return shaderBlob;
}
static const char* GetSemanticNameFromIndex(shader_input_slot_e slot)
{
    switch(slot)
    {
    case kGfxShaderInputPosition:  return "POSITION";
    case kGfxShaderInputNormal:    return "NORMAL";
    case kGfxShaderInputColor:     return "COLOR";
    case kGfxShaderInputTexCoord0: return "TEXCOORD";
    default:
        assert(0);
    };
    return NULL;
}

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
/* Graphics device managment */
graphics_t* gfxCreate(void* window)
{
    DXGI_SWAP_CHAIN_DESC    swapChainDesc   = {0};
    char                    vsProfile[20]   = {0};
    char                    psProfile[20]   = {0};
    RECT                    clientRect      = {0};
    D3D11_RASTERIZER_DESC   rasterDesc      = {0};
    D3D11_SAMPLER_DESC      samplerDesc     = {0};
    D3D11_DEPTH_STENCIL_DESC    depthDesc   = {0};
    D3D11_BLEND_DESC        blendDesc       = {0};
    graphics_t*             device          = NULL;
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
    assert(device == NULL);
    malloc_and_zero(device, graphics_t*);

    device->context     = dxContext;
    device->device      = dxDevice;
    device->swapChain   = dxSwapChain;
    strcpy_s(device->vsProfile, sizeof(device->vsProfile), vsProfile);
    strcpy_s(device->psProfile, sizeof(device->psProfile), psProfile);

    /* Resize to create buffers */
    gfxResize(device, width, height);

    /*
     * Perform other initialization
     */

    /* rasterizer */
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.DepthClipEnable = TRUE;
    hr = ID3D11Device_CreateRasterizerState(device->device, &rasterDesc, &device->solidRasterState);
    assert(SUCCEEDED(hr));
    
    ID3D11DeviceContext_RSSetState(device->context, device->solidRasterState);
    ID3D11DeviceContext_IASetPrimitiveTopology(device->context, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    /* samplers */
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = ID3D11Device_CreateSamplerState(device->device, &samplerDesc, &device->linearSamplerState);
    assert(SUCCEEDED(hr));
    ID3D11DeviceContext_PSSetSamplers(device->context, 0, 1, &device->linearSamplerState);

    /* Depth states */
    depthDesc.DepthEnable = 1;
    depthDesc.StencilEnable = 0;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

    hr = ID3D11Device_CreateDepthStencilState(device->device, &depthDesc, &device->depthWriteTest);
    assert(SUCCEEDED(hr));
    
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr = ID3D11Device_CreateDepthStencilState(device->device, &depthDesc, &device->depthNoWriteTest);
    assert(SUCCEEDED(hr));
    
    depthDesc.DepthEnable = 0;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    hr = ID3D11Device_CreateDepthStencilState(device->device, &depthDesc, &device->depthWriteNoTest);
    assert(SUCCEEDED(hr));
    
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr = ID3D11Device_CreateDepthStencilState(device->device, &depthDesc, &device->depthNoWriteNoTest);
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

    hr = ID3D11Device_CreateBlendState(device->device, &blendDesc, &device->blendAlpha);
    assert(SUCCEEDED(hr));

    blendDesc.RenderTarget[0].BlendEnable   = FALSE;
    blendDesc.RenderTarget[0].SrcBlend      = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend     = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOp       = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha  = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr = ID3D11Device_CreateBlendState(device->device, &blendDesc, &device->blendNoAlpha);
    assert(SUCCEEDED(hr));

    return device;
}
void gfxDestroy(graphics_t* device)
{
    IDXGISwapChain_SetFullscreenState(device->swapChain, 0, NULL);
    SAFE_RELEASE(device->blendAlpha);
    SAFE_RELEASE(device->blendNoAlpha);

    SAFE_RELEASE(device->depthWriteTest);
    SAFE_RELEASE(device->depthNoWriteTest);
    SAFE_RELEASE(device->depthWriteNoTest);
    SAFE_RELEASE(device->depthNoWriteNoTest);

    SAFE_RELEASE(device->linearSamplerState);
    SAFE_RELEASE(device->solidRasterState);

    SAFE_RELEASE(device->renderTargetView);
    SAFE_RELEASE(device->depthStencilView);
    SAFE_RELEASE(device->depthStencilResource);
    SAFE_RELEASE(device->swapChain);
    SAFE_RELEASE(device->context);
    SAFE_RELEASE(device->device);

    free(device);
}
gfx_api_e gfxGetApi(void)
{
    return kDirectX;
}

/* Device contols */
void gfxClear(graphics_t* device)
{
    ID3D11DeviceContext_ClearRenderTargetView(device->context, device->renderTargetView, device->clearColor);
    ID3D11DeviceContext_ClearDepthStencilView(device->context, device->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, device->depthClear, 0);
}
void gfxPresent(graphics_t* device)
{
    HRESULT hr = S_OK;    
    /* TODO: Support occluded present test */
    hr = IDXGISwapChain_Present(device->swapChain, 0, 0 ); 
}
void gfxResize(graphics_t* device, int width, int height)
{
    HRESULT                         hr = S_OK;
    ID3D11Texture2D*                backBuffer = NULL;
    D3D11_TEXTURE2D_DESC            depthTextureDesc;
    D3D11_VIEWPORT                  viewportDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC   depthStencilDesc;

    if(device == NULL)
        return;

    /* Release existing */
    ID3D11DeviceContext_OMSetRenderTargets(device->context, 0, NULL, NULL );
    SAFE_RELEASE(device->renderTargetView);
    SAFE_RELEASE(device->depthStencilView);
    SAFE_RELEASE(device->depthStencilResource);

    IDXGISwapChain_ResizeBuffers(device->swapChain, 2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    /* Create new render target */
    hr = IDXGISwapChain_GetBuffer(device->swapChain, 0, &IID_ID3D11Texture2D, (void**)(&backBuffer));
    assert(SUCCEEDED(hr));
    hr = ID3D11Device_CreateRenderTargetView(device->device, (ID3D11Resource*)backBuffer, NULL, &device->renderTargetView);
    assert(SUCCEEDED(hr));
    SAFE_RELEASE(backBuffer);

    /* Depth stencil texture */
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
    hr = ID3D11Device_CreateTexture2D(device->device, &depthTextureDesc, NULL, &device->depthStencilResource);
    assert(SUCCEEDED(hr));

    /* Depth stencil view */
    depthStencilDesc.Format = depthTextureDesc.Format;
    depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilDesc.Texture2D.MipSlice = 0;
    depthStencilDesc.Flags = 0;
    hr = ID3D11Device_CreateDepthStencilView(device->device, (ID3D11Resource*)device->depthStencilResource, &depthStencilDesc, &device->depthStencilView);
    assert(SUCCEEDED(hr));

    /* Setup viewport */
    viewportDesc.Width    = (float)width;
    viewportDesc.Height   = (float)height;
    viewportDesc.MinDepth = 0.0f;
    viewportDesc.MaxDepth = 1.0f;
    viewportDesc.TopLeftX = 0;
    viewportDesc.TopLeftY = 0;
    ID3D11DeviceContext_RSSetViewports(device->context, 1, &viewportDesc);

    /* Restore render targets */
    ID3D11DeviceContext_OMSetRenderTargets(device->context, 1, &device->renderTargetView, device->depthStencilView );
}


/* Pipeline management */
void gfxSetClearColor(graphics_t* device, float r, float g, float b, float a, float depth)
{
    device->clearColor[0] = r;
    device->clearColor[1] = g;
    device->clearColor[2] = b;
    device->clearColor[3] = a;
    device->depthClear = depth;
}
void gfxSetDepthTest(graphics_t* device, int test, int write)
{
    if(test)
    {
        if(write)
            ID3D11DeviceContext_OMSetDepthStencilState(device->context, device->depthWriteTest, 0);
        else
            ID3D11DeviceContext_OMSetDepthStencilState(device->context, device->depthNoWriteTest, 0);
    }
    else
    {
        if(write)
            ID3D11DeviceContext_OMSetDepthStencilState(device->context, device->depthWriteNoTest, 0);
        else
            ID3D11DeviceContext_OMSetDepthStencilState(device->context, device->depthNoWriteNoTest, 0);
    }
}
void gfxSetAlphaTest(graphics_t* device, int enable)
{
    float blendFactor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    if(enable)
        ID3D11DeviceContext_OMSetBlendState(device->context, device->blendAlpha, blendFactor, 0xFFFFFFFF);
    else
        ID3D11DeviceContext_OMSetBlendState(device->context, device->blendNoAlpha, blendFactor, 0xFFFFFFFF);
}


/* Object creation */
vertex_shader_t* gfxCreateVertexShader(graphics_t* device, const char* filename)
{
    vertex_shader_t*    shader              = NULL;
    ID3D11VertexShader* vertexShader        = NULL;
    ID3DBlob*           vertexShaderBlob    = NULL;
    void*               shaderCode          = NULL;
    size_t              shaderSize          = 0;
    HRESULT             hr                  = S_OK;

    vertexShaderBlob = CompileShader(filename, device->vsProfile);
    assert(vertexShaderBlob);
    shaderCode = ID3D10Blob_GetBufferPointer(vertexShaderBlob);
    shaderSize = ID3D10Blob_GetBufferSize(vertexShaderBlob);
    hr = ID3D11Device_CreateVertexShader(device->device, shaderCode, shaderSize, NULL, &vertexShader);
    assert(SUCCEEDED(hr));

    /* Create new shader */
    malloc_and_zero(shader, vertex_shader_t*);
    shader->shader      = vertexShader;
    shader->shaderBlob  = vertexShaderBlob;

    return shader;
}
pixel_shader_t* gfxCreatePixelShader(graphics_t* device, const char* filename)
{
    pixel_shader_t*     shader          = NULL;
    ID3D11PixelShader*  pixelShader     = NULL;
    ID3DBlob*           pixelShaderBlob = NULL;
    void*               shaderCode      = NULL;
    size_t              shaderSize      = 0;
    HRESULT             hr              = S_OK;

    pixelShaderBlob = CompileShader(filename, device->psProfile);
    assert(pixelShaderBlob);
    shaderCode = ID3D10Blob_GetBufferPointer(pixelShaderBlob);
    shaderSize = ID3D10Blob_GetBufferSize(pixelShaderBlob);
    hr = ID3D11Device_CreatePixelShader(device->device, shaderCode, shaderSize, NULL, &pixelShader);
    assert(SUCCEEDED(hr));

    /* Create new shader */
    malloc_and_zero(shader, pixel_shader_t*);
    shader->shader  = pixelShader;

    return shader;
}
material_t* gfxCreateMaterial(graphics_t* device, vertex_shader_t* vertexShader, pixel_shader_t* pixelShader)
{
    material_t* material = NULL;

    /* Allocate material */
    malloc_and_zero(material, material_t*);    
    material->vertexShader  = vertexShader->shader;
    material->pixelShader   = pixelShader->shader;
    ADD_REF(material->vertexShader);
    ADD_REF(material->pixelShader);
      
    return material;
    UNUSED_PARAMETER(device);
}
mesh_t* gfxCreateMesh(graphics_t* device,
                     vertex_shader_t* vertexShader,
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
    hr = ID3D11Device_CreateBuffer(device->device, &bufferDesc, &initData, &vertexBuffer);
    assert(SUCCEEDED(hr));
    
    /* index buffer */
    bufferDesc.ByteWidth        = (UINT)(indexSize*indexCount);
    bufferDesc.BindFlags        = D3D11_BIND_INDEX_BUFFER;
    initData.pSysMem            = indices;
    hr = ID3D11Device_CreateBuffer(device->device, &bufferDesc, &initData, &indexBuffer);
    assert(SUCCEEDED(hr));

    /* input layout */
    while(layout && layout->slot != kGfxShaderInputNull)
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
    hr = ID3D11Device_CreateInputLayout(device->device, 
                                        inputDesc, 
                                        layoutCount, 
                                        ID3D10Blob_GetBufferPointer(vertexShader->shaderBlob), 
                                        ID3D10Blob_GetBufferSize(vertexShader->shaderBlob), 
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
texture_t* gfxCreateTexture(graphics_t* device, const char* filename)
{
    texture_t*                  texture     = NULL;
    ID3D11ShaderResourceView*   resourceView= NULL;
    HRESULT                     hr          = S_OK;

    hr = D3DX11CreateShaderResourceViewFromFile(device->device, filename, NULL, NULL, &resourceView, NULL); 
    assert(SUCCEEDED(hr));

    /* allocate texture */
    malloc_and_zero(texture, texture_t*);
    texture->resourceView = resourceView;

    return texture;
}
constant_buffer_t* gfxCreateConstantBuffer(graphics_t* device, size_t size, const void* data)
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

    hr = ID3D11Device_CreateBuffer(device->device, (data) ? &bufferDesc : NULL, &initData, &buffer);
    assert(SUCCEEDED(hr));

    /* allocate buffer */
    malloc_and_zero(constantBuffer, constant_buffer_t*);
    constantBuffer->buffer = buffer;

    return constantBuffer;
}

/* object controls */
void gfxUpdateConstantBuffer(graphics_t* device, constant_buffer_t* buffer, size_t size, const void* data)
{
    D3D11_MAPPED_SUBRESOURCE mappedBuffer;
    ID3D11DeviceContext_Map(device->context, (ID3D11Resource*)buffer->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
    memcpy(mappedBuffer.pData, data, size);
    ID3D11DeviceContext_Unmap(device->context, (ID3D11Resource*)buffer->buffer, 0);
}
void gfxBindConstantBufferToIndex(graphics_t* device, material_t* material, const char* bufferName, int index)
{
    UNUSED_PARAMETER(device);
    UNUSED_PARAMETER(material);
    UNUSED_PARAMETER(bufferName);
    UNUSED_PARAMETER(index);
}

/* Object destruction */
void gfxDestroyVertexShader(vertex_shader_t* shader)
{
    SAFE_RELEASE(shader->shaderBlob);
    SAFE_RELEASE(shader->shader);
    free(shader);
}
void gfxDestroyPixelShader(pixel_shader_t* shader)
{
    SAFE_RELEASE(shader->shader);
    free(shader);
}
void gfxDestroyMaterial(material_t* material)
{
    SAFE_RELEASE(material->pixelShader);
    SAFE_RELEASE(material->vertexShader);
    free(material);
}
void gfxDestroyMesh(mesh_t* mesh)
{
    SAFE_RELEASE(mesh->vertexBuffer);
    SAFE_RELEASE(mesh->indexBuffer);
    SAFE_RELEASE(mesh->inputLayout);
    free(mesh);
}
void gfxDestroyTexture(texture_t* texture)
{
    SAFE_RELEASE(texture->resourceView);
    free(texture);
}
void gfxDestroyConstantBuffer(constant_buffer_t* buffer)
{
    SAFE_RELEASE(buffer->buffer);
    free(buffer);
}

/* Render controls */
void gfxSetMaterial(graphics_t* device, material_t* material)
{
    ID3D11DeviceContext_VSSetShader(device->context, material->vertexShader, NULL, 0);
    ID3D11DeviceContext_PSSetShader(device->context, material->pixelShader, NULL, 0);
}
void gfxSetTexture(graphics_t* device, texture_t* texture)
{
    ID3D11DeviceContext_PSSetShaderResources(device->context, 0, 1, &texture->resourceView);
}
void gfxSetVSConstantBuffer(graphics_t* device, constant_buffer_t* buffer, uint32_t index)
{
    ID3D11DeviceContext_VSSetConstantBuffers(device->context, index, 1, &buffer->buffer);
}
void gfxSetPSConstantBuffer(graphics_t* device, constant_buffer_t* buffer, uint32_t index)
{
    ID3D11DeviceContext_PSSetConstantBuffers(device->context, index, 1, &buffer->buffer);
}
void gfxDrawMesh(graphics_t* device, mesh_t* mesh)
{
    UINT    offset = 0;
    DXGI_FORMAT indexFormat = (mesh->indexStride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    ID3D11DeviceContext_IASetInputLayout(device->context, mesh->inputLayout);
    ID3D11DeviceContext_IASetVertexBuffers(device->context, 0, 1, &mesh->vertexBuffer, &mesh->vertexStride, &offset);
    ID3D11DeviceContext_IASetIndexBuffer(device->context, mesh->indexBuffer, indexFormat, 0);
    ID3D11DeviceContext_DrawIndexed(device->context, mesh->indexCount, 0, 0);
}

#endif // #if (GD_API == GD_DIRECTX)
