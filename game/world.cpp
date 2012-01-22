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
const vertex_element_desc_t kPosTexVertexFormat[] =
{
    { kGfxShaderInputPosition,  3 },
    { kGfxShaderInputTexCoord0, 2 },
    { kGfxShaderInputNull,      0 }, 
};

b2Body* s_brickBody = NULL;

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
    memset(_activeBodies, 0, sizeof(_activeBodies));
    _numActiveBodies = 0;

    b2Vec2  gravity(0.0f, -9.8f);
    bool    sleep = true;
    _box2d = new b2World(gravity);
    _box2d->SetAllowSleeping(sleep);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -4.0f);
    b2Body* groundBody = _box2d->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(64.0f, 4.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    //for(int ii=0; ii<100; ++ii)
    //{
    //    AddBrick(-5.0f + (ii/10.0f), 10.0f+1.1f*ii);
    //}

    BuildBuilding();
}
void World::AddBrick(float x, float y)
{
    if(_numActiveBodies == kMaxObjects)
        return;

    int bodyIndex = _numActiveBodies++;
     
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    _activeBodies[bodyIndex] = _box2d->CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    _activeBodies[bodyIndex]->CreateFixture(&fixtureDef);
}
void World::BuildBuilding(void)
{
    float xOffset = -0.5f;
    for(float y = 0.5f; _numActiveBodies < kMaxObjects /*y <= 20.5f*/; y += 1.0f)
    {
        for(float x = -19.0f; x <= 19.0f; x += 2.0f)
        {
            AddBrick(x+xOffset,y);
        }
        xOffset *= -1.0f;
    }
}
void World::Explosion(float x, float y, float radius)
{
    //_activeBodies[0]->ApplyForceToCenter(b2Vec2(0.0f, 100.0f));
    //_activeBodies[0]->ApplyForce(b2Vec2(0.0f, 100.0f));
    //return;
    for(b2Body* b = _box2d->GetBodyList(); b; b = b->GetNext())
    {
        b2Vec2 forcePosition(x,y);
        b2Vec2 bodyPosition = b->GetPosition();
    
        float distance = b2Distance(bodyPosition, forcePosition);
        if(distance > radius) continue;
    
        float force = (radius - distance) / radius;
        force *= 100.0f;
        b2Vec2 forceDirecton = bodyPosition-forcePosition;
        forceDirecton.Normalize();
        forceDirecton *= force;
    
        b->ApplyLinearImpulse(forceDirecton, bodyPosition);
    }
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

void World::Update(float elapsedTime)
{
    int32_t velocityIterations = 6;
    int32_t positionIterations = 2;
    _box2d->Step(elapsedTime, velocityIterations, positionIterations);
}
void World::Render(void)
{
    Matrix4 identity = Matrix4Identity();
    /* Render background */
    gfxSetMaterial(_graphics, _material);
    gfxUpdateConstantBuffer(_graphics, _perFrameConstantBuffer, sizeof(identity), &identity);
    gfxUpdateConstantBuffer(_graphics, _perObjectConstantBuffer, sizeof(identity), &identity);
    gfxSetVSConstantBuffer(_graphics, _perFrameConstantBuffer, 0);
    gfxSetVSConstantBuffer(_graphics, _perObjectConstantBuffer, 1);
    gfxSetTexture(_graphics, _backgroundTexture);
    gfxDrawMesh(_graphics, _quadMesh);

    /* Render bricks */
    Matrix4 projMatrix = Matrix4OrthographicOffCenterLH(-64.0f, 64.0f, 120.0f, -8.0f, -1.0f, 1.0f);
    gfxUpdateConstantBuffer(_graphics, _perFrameConstantBuffer, sizeof(projMatrix), &projMatrix);
    gfxSetTexture(_graphics, _brickTexture);
    for(int ii=0; ii<_numActiveBodies; ++ii)
    {
        Matrix4 worldMatrix = Matrix4RotationZ(_activeBodies[ii]->GetAngle());
        //worldMatrix = Matrix4MatrixMultiply(identity, Matrix4Scale(0.5f, 0.5f, 1.0f));
        worldMatrix = Matrix4MatrixMultiply(Matrix4Scale(1.0f, 0.5f, 1.0f), worldMatrix);
        b2Vec2 pos = _activeBodies[ii]->GetPosition();
        worldMatrix.r3.x = pos.x;
        worldMatrix.r3.y = pos.y;
        gfxUpdateConstantBuffer(_graphics, _perObjectConstantBuffer, sizeof(worldMatrix), &worldMatrix);
        gfxDrawMesh(_graphics, _quadMesh);
    }
}
void World::SetGraphicsDevice(graphics_t* graphics)
{
    _graphics = graphics;
}
