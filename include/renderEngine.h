/*
 * renderEngine.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/4/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_renderEngine_h__
#define __RiotLib_renderEngine_h__

/* C headers */
#include <stdint.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "vm.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
class GraphicsDevice;
typedef int mesh_id_t;
typedef int vertex_shader_id_t;
typedef int pixel_shader_id_t;
typedef int material_id_t;
typedef int texture_id_t;

struct render_engine_params_t
{
    int graphicsApi;
};

struct render_command_t
{
    Matrix4         world;
    int             worldView;
    mesh_id_t       mesh;
    texture_id_t    texture;
};

namespace ProjectionType
{
    enum Enum
    {
        kOrthographic,
        kPerspective,
    };
}

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
namespace RenderEngine
{
    void Init(const render_engine_params_t& params);
    void Shutdown(void);

    void Resize(int width, int height);

    void Frame(void);

    GraphicsDevice* GetGfxDevice(void);

    mesh_id_t CreateMesh(const char* filename);
    mesh_id_t CreateMesh(   uint32_t vertexCount,
                            uint32_t indexCount,
                            size_t vertexSize,
                            size_t indexSize,
                            const void* vertices,
                            const void* indices);
    texture_id_t CreateTexture(const char* filename);

    void SetWorldViewMatrix(const Matrix4& view);
    void SetWorldProjectionType(ProjectionType::Enum type, float bottom = -0.5f, float top = 0.5f);

    void Render(int worldView, const Matrix4& world, mesh_id_t mesh, texture_id_t texture);
};

#endif /* include guard */
