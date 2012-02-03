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
#include "cJSON.h"
#include "file.h"

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
    : _quadMesh(NULL)
    , _backgroundTexture(NULL)
    , _brickTexture(NULL)
    , _material(NULL)
    , _box2d(NULL)
    , _numActiveEntities(0)
{
}
void World::Create(void)
{
    /*
     * Graphics initialization
     */
    char json[1024] = {0};
    fileLoadAndRead(json, sizeof(json), "Assets/gameData.json");
    cJSON* root = cJSON_Parse(json);
    cJSON* assets = cJSON_GetObjectItem(root, "assets");
    cJSON* textures = cJSON_GetObjectItem(assets, "textures");
    cJSON* shaders = cJSON_GetObjectItem(assets, "shaders");

    const char* groundTexture = cJSON_GetObjectItem(textures, "groundTexture")->valuestring;
    const char* brickTexture = cJSON_GetObjectItem(textures, "brickTexture")->valuestring;
    const char* woodTexture = cJSON_GetObjectItem(textures, "woodTexture")->valuestring;

    const char* vertexShaderFilename = cJSON_GetObjectItem(shaders, "vertexShader")->valuestring;
    const char* pixelShaderFilename = cJSON_GetObjectItem(shaders, "pixelShader")->valuestring;


    /* Create graphics objects */
    vertex_shader_id_t vertexShader = renderCreateVertexShader(vertexShaderFilename);
    pixel_shader_id_t pixelShader = renderCreatePixelShader(pixelShaderFilename);
    _material = renderCreateMaterial(vertexShader, pixelShader);

    /* Create mesh */
    _quadMesh = renderCreateMesh(   vertexShader,
                                    kVtxFmtPosTex,
                                    ARRAY_LENGTH(kQuadVerts),
                                    ARRAY_LENGTH(kQuadIndices),
                                    sizeof(kQuadVerts[0]),
                                    sizeof(kQuadIndices[0]),
                                    kQuadVerts, 
                                    kQuadIndices);
    /* textures */
    _backgroundTexture = renderCreateTexture(groundTexture);
    _brickTexture = renderCreateTexture(brickTexture);
    _woodTexture = renderCreateTexture(woodTexture);

    /*
     * Game initialization
     */
    cJSON_Delete(root);
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

    
    //groundBodyDef.position.Set(-64.5, 0.0f);
    //groundBody = _box2d->CreateBody(&groundBodyDef);
    //groundBox.SetAsBox(0.5f, 200.0f);
    //groundBody->CreateFixture(&groundBox, 0.0f);
    //
    //groundBodyDef.position.Set(64.5, 0.0f);
    //groundBody = _box2d->CreateBody(&groundBodyDef);
    //groundBox.SetAsBox(0.5f, 200.0f);
    //groundBody->CreateFixture(&groundBox, 0.0f);
    //
    //
    //groundBodyDef.position.Set(0.0f, 120.5f);
    //groundBody = _box2d->CreateBody(&groundBodyDef);
    //groundBox.SetAsBox(200.0f, 0.5f);
    //groundBody->CreateFixture(&groundBox, 0.0f);

    BuildBuilding();
}
void World::BuildBuilding(void)
{
    int BrickEntityIndex = _numActiveEntities++;
    BrickEntity::CreateBrickEntity(   &_activeEntities[BrickEntityIndex], _box2d, 
                            _woodTexture, _quadMesh, 
                            -10.0f, 7.5f, 
                            1.0f, 15.0f, 
                            kMaterialProperties[kWood].density, kMaterialProperties[kWood].friction);
                            
    BrickEntityIndex = _numActiveEntities++;
    BrickEntity::CreateBrickEntity(   &_activeEntities[BrickEntityIndex], _box2d, 
                            _woodTexture, _quadMesh, 
                            10.0f, 7.5f, 
                            1.0f, 15.0f, 
                            kMaterialProperties[kWood].density, kMaterialProperties[kWood].friction);
                            
    BrickEntityIndex = _numActiveEntities++;
    BrickEntity::CreateBrickEntity(   &_activeEntities[BrickEntityIndex], _box2d, 
                            _woodTexture, _quadMesh, 
                            0.0f, 7.5f, 
                            1.0f, 15.0f, 
                            kMaterialProperties[kWood].density, kMaterialProperties[kWood].friction);
                            
    BrickEntityIndex = _numActiveEntities++;
    BrickEntity::CreateBrickEntity(   &_activeEntities[BrickEntityIndex], _box2d, 
                            _woodTexture, _quadMesh, 
                            0.0f, 15.5f, 
                            128.0f, 1.0f, 
                            kMaterialProperties[kWood].density, kMaterialProperties[kWood].friction);

    int towerWidth = 64;
    float startX = -63.0f;
    float y = 16.5f;
    while(towerWidth)
    {
        int ii = 0;
        float x = startX;
        for(; ii < towerWidth; ++ii, x += 2.0f)
        {
            if(_numActiveEntities >= kMaxEntities)
                break;
            BrickEntityIndex = _numActiveEntities++;
            BrickEntity::CreateBrickEntity(   &_activeEntities[BrickEntityIndex], _box2d, 
                                    _brickTexture, _quadMesh, 
                                    x, y, 
                                    2.0f, 1.0f, 
                                    kMaterialProperties[kBrick].density, kMaterialProperties[kBrick].friction);
            
        }
        y += 1.0f;
        startX += 1.0f;
        --towerWidth;
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
void World::Resize(float width, float height)
{
}
void World::Render(void)
{
    Matrix4 identity = Matrix4Identity();
    Matrix4 projMatrix = Matrix4OrthographicOffCenterLH(-64.0f, 64.0f, 120.0f, -8.0f, -1.0f, 1.0f);
    renderSetViewProj(1, &projMatrix.r0.x);
    renderSetViewProj(0, &identity.r0.x);
    /* Render background */
    Matrix4 worldMatrix  = Matrix4Scale(2.0f,2.0f,1.0f);
    renderSubmitDraw(0, _material, _backgroundTexture, &worldMatrix.r0.x, _quadMesh);

    /* Render bricks */
    for(int ii=0; ii<_numActiveEntities; ++ii)
    {
        _activeEntities[ii].Render();
    }
}