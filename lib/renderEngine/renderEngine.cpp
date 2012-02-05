/*
 * renderEngine.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/4/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "renderEngine.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "graphicsDevice.h"
#include "system.h"
#include "utility.h"
#include "global.h"
#include "cJSON.h"
#include "file.h"

#define INT_FROM_JSON(json, objectName)                         \
    {                                                           \
        cJSON* _json_ = cJSON_GetObjectItem(json, #objectName); \
        if(_json_)                                              \
            objectName = _json_->valueint;                      \
    }

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
enum
{
    kMaxMeshes = 16,
    kMaxTextures = 16,
    kMaxRenderCommands = 1024*16
};

template<class T, int kMaxCount>
class ResourceList
{
/* Methods */
public:
    ResourceList()
        : _activeItems(0)
    {
    }

    T& operator[](int index) 
    {
        assert(index >= 0 || index < kMaxCount);
        return _items[index]; 
    }
    int GetIndex(const char* string) const
    {
        StringHash hash(string);
        for(int ii=0; ii<_activeItems; ++ii)
        {
            if(hash == _names[ii])
                return ii;
        }
        return -1;
    }

    int Add(const T& item, const char* name)
    {
        assert(_activeItems < kMaxCount);
        int index = _activeItems++;
        _items[index] = item;
        _names[index] = name;
        return index;
    }

    int activeItems(void) const { return _activeItems; }

    void Empty(void) { _activeItems = 0; }
/* Members */
private:
    T           _items[kMaxCount];
    StringHash  _names[kMaxCount];
    int         _activeItems;
};

const vertex_element_desc_t kVertexFormats[][16] =
{
    { /* kVtxFmtPosTex */
        { ShaderInputSlot::kPosition,  3 },
        { ShaderInputSlot::kTexCoord0, 2 },
        { ShaderInputSlot::kNull,      0 },
    },
};

/*******************************************************************\
Internal variables
\*******************************************************************/
GraphicsDevice*                         _graphicsDevice = NULL;
ResourceList<mesh_t*, kMaxMeshes>       _meshes;
ResourceList<texture_t*, kMaxTextures>  _textures;
vertex_shader_t*                        _vertexShader;
pixel_shader_t*                         _pixelShader;
material_t*                             _material;
render_command_t                        _renderCommands[kMaxRenderCommands];
int                                     _numRenderCommands;
constant_buffer_t*                      _viewProjConstBuffer;
constant_buffer_t*                      _worldConstBuffer;

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
namespace RenderEngine
{

void Init(const render_engine_params_t& params)
{
    _graphicsDevice = GraphicsDevice::Create((GraphicsAPI::Enum)params.graphicsApi, System::GetWindow());
    _graphicsDevice->SetClearColor(0.67f, 0.23f, 0.15f, 1.0f, 1.0f);
    _graphicsDevice->SetDepthTest(0, 0);
    _graphicsDevice->SetAlphaTest(1);
    _graphicsDevice->Clear();

    // Create the material
    _vertexShader = _graphicsDevice->CreateVertexShader("assets/postex.vsh");
    _pixelShader = _graphicsDevice->CreatePixelShader("assets/tex.psh");
    _material = _graphicsDevice->CreateMaterial(_vertexShader, _pixelShader);

    // Apply the material
    _graphicsDevice->SetMaterial(_material);

    // Create constant buffers
    Matrix4 ident = Matrix4Identity();
    _viewProjConstBuffer = _graphicsDevice->CreateConstantBuffer(sizeof(Matrix4), &ident);
    _worldConstBuffer = _graphicsDevice->CreateConstantBuffer(sizeof(Matrix4), &ident);

    _graphicsDevice->SetVSConstantBuffer(_viewProjConstBuffer, 0);
    _graphicsDevice->SetVSConstantBuffer(_worldConstBuffer, 1);

    // Create temp texture
    _textures.Add(_graphicsDevice->CreateTexture("assets/ground.png"), "assets/ground.png");

    _numRenderCommands = 0;
}

void Shutdown(void)
{
    for(int ii=0; ii<_meshes.activeItems(); ++ii)
        _graphicsDevice->DestroyMesh(_meshes[ii]);
    for(int ii=0; ii<_textures.activeItems(); ++ii)
        _graphicsDevice->DestroyTexture(_textures[ii]);

        
    _graphicsDevice->DestroyConstantBuffer(_viewProjConstBuffer);
    _graphicsDevice->DestroyConstantBuffer(_worldConstBuffer);
    _graphicsDevice->DestroyMaterial(_material);
    _graphicsDevice->DestroyVertexShader(_vertexShader);
    _graphicsDevice->DestroyPixelShader(_pixelShader);

    _meshes.Empty();
    _textures.Empty();
    _graphicsDevice->Destroy();
    _graphicsDevice = NULL;
    _numRenderCommands = 0;
}

void Resize(int width, int height)
{
    _graphicsDevice->Resize(width, height);
}

void Frame(void)
{
    // Rendering
    _graphicsDevice->Clear();

    Matrix4 ident = Matrix4Identity();
    Render(ident, ident, 0, 0);

    // Render
    for(int ii=0; ii<_numRenderCommands; ++ii)
    {
        const render_command_t& command = _renderCommands[ii];
        _graphicsDevice->SetMaterial(_material);
        _graphicsDevice->UpdateConstantBuffer(_viewProjConstBuffer, sizeof(Matrix4), &command.viewProj);
        _graphicsDevice->UpdateConstantBuffer(_worldConstBuffer, sizeof(Matrix4), &command.world);
        _graphicsDevice->SetVSConstantBuffer(_viewProjConstBuffer, 0);
        _graphicsDevice->SetVSConstantBuffer(_worldConstBuffer, 1);
        _graphicsDevice->SetTexture(_textures[command.texture]);
        _graphicsDevice->DrawMesh(_meshes[command.mesh]);
    }

    _numRenderCommands = 0;

    _graphicsDevice->Present();
}

mesh_id_t CreateMesh(const char* filename)
{
    // Get the extension
    const char* findExt = filename;
    const char* extension = 0;
    while(*findExt != '\0')
    {
        if(*findExt == '.')
            extension = findExt+1;
        findExt++;
    }

    const char tempJson[] = 
    "{\n"
    "   \"vertexCount\" : 4,\n"
    "   \"indexCount\"  : 6,\n"
    "   \"vertexSize\"  : 20,\n"
    "   \"indexSize\"   : 2,\n"
    "   \"vertices\"    : [\n"
    "       -0.5, -0.5, 0.0,   0.0, 1.0,\n"
    "        0.5, -0.5, 0.0,   1.0, 1.0,\n"
    "       -0.5,  0.5, 0.0,   0.0, 0.0,\n"
    "        0.5,  0.5, 0.0,   1.0, 0.0\n"
    "   ],\n"
    "   \"indices\"     : [\n"
    "       0, 2, 1,    \n"
    "       2, 3, 1 ]\n"
    "}\n";

    int     vertexCount;
    int     indexCount;
    int     vertexSize;
    int     indexSize;
    void*   vertices;
    void*   indices;

    StringHash extensionHash(extension);
    if(extensionHash.hash == StringHash("json").hash)
    {
        char fileBuffer[1024*8];
        FileLoadAndRead(fileBuffer, sizeof(fileBuffer), filename);
        cJSON* objectRoot = cJSON_Parse(fileBuffer);
        INT_FROM_JSON(objectRoot, vertexCount);
        INT_FROM_JSON(objectRoot, indexCount);
        INT_FROM_JSON(objectRoot, vertexSize);
        INT_FROM_JSON(objectRoot, indexSize);

        vertices = new char[vertexSize*vertexCount];
        float* fvertices = (float*)vertices;
        cJSON* jsonVertices = cJSON_GetObjectItem(objectRoot, "vertices");
        for(int ii=0; ii<cJSON_GetArraySize(jsonVertices); ++ii)
        {
            fvertices[ii] = (float)cJSON_GetArrayItem(jsonVertices, ii)->valuedouble;
        }

        indices = new char[indexSize*indexCount];
        short* sindices = (short*)indices;
        cJSON* jsonIndices = cJSON_GetObjectItem(objectRoot, "indices");
        for(int ii=0; ii<cJSON_GetArraySize(jsonIndices); ++ii)
        {
            sindices[ii] = cJSON_GetArrayItem(jsonIndices, ii)->valueint;
        }

        cJSON_Delete(objectRoot);
    }
    else
    {
    }

    mesh_id_t mesh = _meshes.Add(_graphicsDevice->CreateMesh(   _vertexShader,
                                                                kVertexFormats[0],
                                                                vertexCount,
                                                                indexCount,
                                                                vertexSize, 
                                                                indexSize,
                                                                vertices,
                                                                indices), filename);

    delete [] vertices;
    delete [] indices;

    return mesh;
}
texture_id_t CreateTexture(const char* filename)
{
    texture_id_t texture = _textures.Add(_graphicsDevice->CreateTexture(filename), filename);
    return texture;
}

void Render(const Matrix4& viewProj, const Matrix4& world, mesh_id_t mesh, texture_id_t texture)
{
    int index = _numRenderCommands++;
    render_command_t& command = _renderCommands[index];
    command.viewProj = viewProj;
    command.world = world;
    command.mesh = mesh;
    command.texture = texture;
}

} // namespace RenderEngine
