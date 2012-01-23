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
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } },
    { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } },
    { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } },
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

const material_properties_t   kMaterialProperties[kNUM_MATERIALS] =
{
    { 2000.0f, 0.65f }, // Brick
    {  750.0f, 0.30f }, // Wood
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
World::World()
    : _graphics(NULL)
    , _quadMesh(NULL)
    , _backgroundTexture(NULL)
    , _brickTexture(NULL)
    , _material(NULL)
    , _perFrameConstantBuffer(NULL)
    , _perObjectConstantBuffer(NULL)
    , _box2d(NULL)
    , _numActiveEntities(0)
{
}
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
    _woodTexture = gfxCreateTexture(_graphics, "assets/wood.png");

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
    Reset();
}
void World::Reset(void)
{
    /*
     * Release
     */
    SAFE_DELETE(_box2d);

    /*
     * Initialize
     */
    Entity::SetConstantBuffer(_perObjectConstantBuffer);
    Entity::SetGraphics(_graphics);
    memset(_activeEntities, 0, sizeof(_activeEntities));
    _numActiveEntities = 0;
    
    // World
    b2Vec2  gravity(0.0f, -9.8f);
    bool    sleep = true;
    _box2d = new b2World(gravity);
    _box2d->SetAllowSleeping(sleep);

    // Ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -4.0f);
    b2Body* groundBody = _box2d->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(128.0f, 4.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    BuildBuilding();
}
void World::BuildBuilding(void)
{
    float xOffset = -0.5f;
    for(float y = 0.5f; _numActiveEntities < kMaxEntities /*y <= 20.5f*/; y += 1.0f)
    {
        for(float x = -19.0f; x <= 19.0f; x += 2.0f)
        {
            if(_numActiveEntities == kMaxEntities)
                break;
            int entityIndex = _numActiveEntities++;

            if(y < 10.0f)
                Entity::CreateEntity(   &_activeEntities[entityIndex], _box2d, 
                                        _woodTexture, _quadMesh, 
                                        x+xOffset, y, 
                                        2.0f, 1.0f, 
                                        kMaterialProperties[kWood].density, kMaterialProperties[kWood].friction);
            else
                Entity::CreateEntity(   &_activeEntities[entityIndex], _box2d, 
                                        _brickTexture, _quadMesh, 
                                        x+xOffset, y, 
                                        2.0f, 1.0f, 
                                        kMaterialProperties[kBrick].density, kMaterialProperties[kBrick].friction);
        }
        xOffset *= -1.0f;
    }
}
void World::Explosion(float x, float y, float radius, float force)
{
    float origForce = force;
    for(b2Body* b = _box2d->GetBodyList(); b; b = b->GetNext())
    {
        b2Vec2 forcePosition(x,y);
        b2Vec2 bodyPosition = b->GetPosition();
    
        float distance = b2Distance(bodyPosition, forcePosition);
        if(distance > radius) continue;
    
        force = origForce * ((radius - distance) / radius);
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
    gfxDestroyTexture(_woodTexture);
    gfxDestroyConstantBuffer(_perFrameConstantBuffer);
    gfxDestroyConstantBuffer(_perObjectConstantBuffer);
}

void World::Update(float elapsedTime)
{
    int32_t velocityIterations = 6;
    int32_t positionIterations = 2;
    _box2d->Step(elapsedTime, velocityIterations, positionIterations);
}
void World::ConvertToWorldPos(float* x, float* y)
{
    float origX = *x;
    float origY = *y;
    *x = origX * 64.0f;
    *y = (origY * 64.0f) + 56.0f;
}

void World::Render(void)
{
    Matrix4 identity = Matrix4Identity();
    Matrix4 worldMatrix  = Matrix4Scale(2.0f,2.0f,1.0f);
    /* Render background */
    gfxSetMaterial(_graphics, _material);
    gfxUpdateConstantBuffer(_graphics, _perFrameConstantBuffer, sizeof(identity), &identity);
    gfxUpdateConstantBuffer(_graphics, _perObjectConstantBuffer, sizeof(worldMatrix), &worldMatrix);
    gfxSetVSConstantBuffer(_graphics, _perFrameConstantBuffer, 0);
    gfxSetVSConstantBuffer(_graphics, _perObjectConstantBuffer, 1);
    gfxSetTexture(_graphics, _backgroundTexture);
    gfxDrawMesh(_graphics, _quadMesh);

    /* Render bricks */
    Matrix4 projMatrix = Matrix4OrthographicOffCenterLH(-64.0f, 64.0f, 120.0f, -8.0f, -1.0f, 1.0f);
    gfxUpdateConstantBuffer(_graphics, _perFrameConstantBuffer, sizeof(projMatrix), &projMatrix);
    for(int ii=0; ii<_numActiveEntities; ++ii)
    {
        _activeEntities[ii].Render();
    }
}
void World::SetGraphicsDevice(graphics_t* graphics)
{
    _graphics = graphics;
}
