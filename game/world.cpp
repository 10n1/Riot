/*
 * world.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "world.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"
#include "vm.h"
#include "graphicsDevice/graphicsDevice.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
struct pos_tex_vertex_t
{
    float pos[3];
    float tex[2];
};

/*******************************************************************\
Internal variables
\*******************************************************************/
const pos_tex_vertex_t kQuadVerts[] = 
{
    { { -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f } },
    { {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f } },
    { { -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f } },
    { {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f } },
};
const int kQuadIndices[] =
{
    0,2,1,
    2,3,1,
};
static const vertex_element_desc_t kPosTexVertexFormat[] =
{
    { kGfxShaderInputPosition,  3 },
    { kGfxShaderInputTexCoord0, 2 },
    { kGfxShaderInputNull,      0 }, 
};

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
void World::Create(void)
{
    /*
     * Graphics initialization
     */

    /* Create graphics objects */
    vertex_shader_t* vertexShader = gfxCreateVertexShader(_graphics, "assets/pos.vsh");
    pixel_shader_t* pixelShader = gfxCreatePixelShader(_graphics, "assets/texture.psh");
    _material = gfxCreateMaterial(_graphics, vertexShader, pixelShader);

    /* Create mesh */
    _quadMesh = gfxCreateMesh(  _graphics,
                                vertexShader,
                                kPosTexVertexFormat,
                                ARRAY_LENGTH(kQuadVerts),
                                ARRAY_LENGTH(kQuadIndices),
                                sizeof(kQuadVerts[0]),
                                sizeof(kQuadIndices[0]),
                                kQuadVerts, 
                                kQuadIndices);
    /* textures */
    _backgroundTexture = gfxCreateTexture(_graphics, "assets/ground.png");
    _brickTexture = gfxCreateTexture(_graphics, "assets/brick.png");

    /* Constant buffers */
    gfxBindConstantBufferToIndex(_graphics, _material, "PerFrame", 0);
    gfxBindConstantBufferToIndex(_graphics, _material, "PerObject", 1);
    Matrix4 projMatrix = Matrix4OrthographicOffCenterLH(-1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);
    projMatrix = Matrix4Identity();
    _perFrameConstantBuffer = gfxCreateConstantBuffer(_graphics, sizeof(projMatrix), &projMatrix);
    Matrix4 identity = Matrix4Identity();
    _perObjectConstantBuffer = gfxCreateConstantBuffer(_graphics, sizeof(identity), &identity);

    /* Release shaders */
    gfxDestroyVertexShader(vertexShader);
    gfxDestroyPixelShader(pixelShader);

    /*
     * Game initialization
     */
    b2Vec2  gravity(0.0f, -9.8f);
    bool    sleep = true;
    _box2d = new b2World(gravity);
    _box2d->SetAllowSleeping(sleep);
}
void World::Destroy(void)
{
    delete _box2d;

    gfxDestroyMaterial(_material);
    gfxDestroyMesh(_quadMesh);
    gfxDestroyTexture(_backgroundTexture);
    gfxDestroyTexture(_brickTexture);
    gfxDestroyConstantBuffer(_perFrameConstantBuffer);
    gfxDestroyConstantBuffer(_perObjectConstantBuffer);
}

void World::Update(float)
{
}
void World::Render(void)
{
    Matrix4 identity = Matrix4Identity();
    /* Render background */
    gfxSetMaterial(_graphics, _material);
    gfxUpdateConstantBuffer(_graphics, _perFrameConstantBuffer, sizeof(identity), &identity);
    gfxSetVSConstantBuffer(_graphics, _perFrameConstantBuffer, 0);
    gfxSetVSConstantBuffer(_graphics, _perObjectConstantBuffer, 1);
    gfxSetTexture(_graphics, _backgroundTexture);
    gfxDrawMesh(_graphics, _quadMesh);

    /* Render bricks */
    Matrix4 projMatrix = Matrix4OrthographicOffCenterLH(-60.0f, 60.0f, 100.0f, -20.0f, -1.0f, 1.0f);
    //Matrix4 projMatrix = Matrix4OrthographicOffCenterLH(-1, 1, 1, -1, -1.0f, 1.0f);
    gfxUpdateConstantBuffer(_graphics, _perFrameConstantBuffer, sizeof(projMatrix), &projMatrix);
    gfxSetTexture(_graphics, _brickTexture);
    gfxDrawMesh(_graphics, _quadMesh);
}
void World::SetGraphicsDevice(graphics_t* graphics)
{
    _graphics = graphics;
}
