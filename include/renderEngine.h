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

namespace VertexFormat
{
    enum Enum
    {
        kPosTex,
        kPosNormTex,

        kMAX_VERTEX_FORMATS
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

    vertex_shader_id_t CreateVertexShader(const char* filename);
    pixel_shader_id_t CreatePixelShader(const char* filename);
    material_id_t CreateMaterial(vertex_shader_id_t vertexShader, pixel_shader_id_t pixelShader);
    mesh_id_t CreateMesh(   vertex_shader_id_t vertexShader,
                            VertexFormat::Enum format,
                            uint32_t vertexCount,
                            uint32_t indexCount,
                            size_t vertexSize,
                            size_t indexSize,
                            const void* vertices,
                            const void* indices);
    texture_id_t CreateTexture(const char* filename);
};

#endif /* include guard */
