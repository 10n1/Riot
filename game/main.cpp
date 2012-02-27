/*
 * main.cpp
 * Riot
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

/* C headers */
#include <stdio.h>
/* C++ headers */
/* External headers */
#include "box2d/box2d.h"
/* Internal headers */
#include "global.h"
#include "core.h"
#include "renderEngine.h"
#include "entity.h"
#include "component.h"
#include "timer.h"
#include "terrain.h"
#include "perlin.h"
#include "system.h"

float* _terrainHeights = NULL;
const int terrainSize = 512;
const int terrainHeight = 250;
extern Perlin perlin;
void* terrainVertices = NULL;
int terrainMesh = 0;

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
const char kEngineJson[] = 
"{\n"
"   \"createWindow\" : true,\n"
"   \"windowWidth\"  : 1024,\n"
"   \"windowHeight\" : 768,\n"
"   \"graphicsApi\"  : \"directx\"\n"
"}\n";

timer_t _timer;
EntitySystem*   _entitySystem = NULL;
RenderComponent*    _renderComponent = NULL;
PhysicsComponent*   _physicsComponent = NULL;
CameraComponent*    _cameraComponent = NULL;
FirstPersonComponent*   _firstPersonComponent = NULL;
DigComponent*           _digComponent = NULL;
int groundEntity = 0;

/*******************************************************************\
Internal functions
\*******************************************************************/
void Initialize(void)
{
    timerInit(&_timer);
    _terrainHeights = GenerateTerrain(terrainSize);

    int buildingSize = 10;
    int minX = terrainSize/2 - buildingSize/2;
    int minZ = terrainSize/2 - buildingSize/2 + 25;
    int maxX = minX+buildingSize;
    int maxZ = minZ+buildingSize;

    float avgHeight = 0.0f;
    int count = 0;
    for(int xx=minX; xx <= maxX; xx++)
    {
        for(int zz=minZ; zz <= maxZ; zz++)
        {
            avgHeight += TERRAIN_INDEX(xx,zz);
            count++;
        }
    }
    avgHeight /= count;
    for(int xx=minX-1; xx <= maxX+1; xx++)
    {
        for(int zz=minZ-1; zz <= maxZ+1; zz++)
        {
            TERRAIN_INDEX(xx,zz) = avgHeight;
        }
    }

    _entitySystem = new EntitySystem();
    _renderComponent = new RenderComponent();
    _physicsComponent = new PhysicsComponent();
    _cameraComponent = new CameraComponent();
    _firstPersonComponent = new FirstPersonComponent();
    _digComponent = new DigComponent();

    terrainMesh = GenerateTerrainMesh(_terrainHeights, terrainSize, &terrainVertices);
    /* Create background object */
    //int backgroundEntity = _entitySystem->CreateEntity();
    RenderComponentParams renderParams;
    renderParams.mesh = terrainMesh;// RenderEngine::CreateMesh("assets/quadMesh.json");
    renderParams.texture = 0;
    renderParams.worldView = 0;
    int renderComponent;// = _renderComponent->CreateComponent(&renderParams);
    
    //_entitySystem->AttachComponent(backgroundEntity, _renderComponent, renderComponent);

    // Create ground
    groundEntity = _entitySystem->CreateEntity();
    renderParams.texture = RenderEngine::CreateTexture("assets/grass.jpg");
    renderParams.worldView = 1;
    renderComponent = _renderComponent->CreateComponent(&renderParams);
    _entitySystem->AttachComponent(groundEntity, _renderComponent, renderComponent);
    //_entitySystem->GetEntity(groundEntity)->transform.orientation = QuatRotationX(DegToRad(90.0f));
    //_entitySystem->GetEntity(groundEntity)->transform.scale = 100.0f;

    // Create camera
    int cameraEntity = _entitySystem->CreateEntity();
    int cameraComponent = _cameraComponent->CreateComponent(NULL);
    int firstPersonComponent = _firstPersonComponent->CreateComponent(NULL);
    int digComponent = _digComponent->CreateComponent(NULL);
    _entitySystem->AttachComponent(cameraEntity, _cameraComponent, cameraComponent);
    _entitySystem->AttachComponent(cameraEntity, _firstPersonComponent, firstPersonComponent);
    _entitySystem->AttachComponent(cameraEntity, _digComponent, digComponent);
    Entity* entity = _entitySystem->GetEntity(cameraEntity);
    entity->transform.position.x = terrainSize/2;
    entity->transform.position.y = avgHeight+10.0f;
    entity->transform.position.z = terrainSize/2-50.0f;
    // Create bricks
    int brickIndex = 0;
    int towerWidth = 0;
    float startX = -63.0f;
    float y = 0.5f;
    renderParams.mesh = RenderEngine::CreateMesh("assets/cubemesh.json");
    //renderParams.mesh = RenderEngine::CreateMesh("assets/drone.sdkmesh.colony");
    renderParams.texture = RenderEngine::CreateTexture("assets/brick.png");
    while(towerWidth)
    {
        int ii = 0;
        float x = startX;
        for(; ii < towerWidth; ++ii, x += 2.0f)
        {
            int brickEntity = _entitySystem->CreateEntity();
            PhysicsComponentParams params;
            params.halfHeight = 0.5f;
            params.halfWidth = 0.5f;
            params.halflength = 0.5f;
            params.mass = 1.0f;
            params.transform = TransformZero();
            params.transform.position.x = x;
            params.transform.position.y = y*2;
            params.transform.position.y += 300.0f;
            params.transform.position.x += terrainSize/2;
            params.transform.position.z += terrainSize/2 + 30;
            int physicsComponent = _physicsComponent->CreateComponent(&params);
            int renderComponent = _renderComponent->CreateComponent(&renderParams);
            _entitySystem->AttachComponent(brickEntity, _physicsComponent, physicsComponent);
            _entitySystem->AttachComponent(brickEntity, _renderComponent, renderComponent);
        }
        y += 1.0f;
        startX += 1.0f;
        --towerWidth;
    }

    // Building bricks
    float level = avgHeight+0.5f;
    for(int y=0; y < 3; ++y)
    {
        for(int xx=minX; xx <= maxX; xx++)
        {
            for(int zz=minZ; zz <= maxZ; zz++)
            {
                if(xx > minX && xx < maxX && zz > minZ && zz < maxZ)
                    continue;
                int brickEntity = _entitySystem->CreateEntity();
                PhysicsComponentParams params;
                params.halfHeight = 0.5f;
                params.halfWidth = 0.5f;
                params.halflength = 0.5f;
                params.mass = 1.0f;
                params.transform = TransformZero();
                params.transform.position.x = xx;
                params.transform.position.y = level;
                params.transform.position.z = zz;
                int physicsComponent = _physicsComponent->CreateComponent(&params);
                int renderComponent = _renderComponent->CreateComponent(&renderParams);
                _entitySystem->AttachComponent(brickEntity, _physicsComponent, physicsComponent);
                _entitySystem->AttachComponent(brickEntity, _renderComponent, renderComponent);
            }
        }
        level += 1.0f;
    }

    for(int ii=0; ii<1024*4; ++ii)
    {
        float x = rand()/(float)RAND_MAX * terrainSize;
        float z = rand()/(float)RAND_MAX * terrainSize;
        while(x > minX && x < maxX && z > minZ && z < maxZ)
        {
            x = rand()/(float)RAND_MAX * terrainSize;
            z = rand()/(float)RAND_MAX * terrainSize;
        }
        float y = perlin.Get(x/terrainSize, z/terrainSize);
        float scale = 10.0f;
        int treeEntity = _entitySystem->CreateEntity();
        Transform& t = _entitySystem->GetEntity(treeEntity)->transform;
        t.position.x = x;
        t.position.y = y;
        t.position.z = z;
        t.scale = scale;
        renderParams.mesh = RenderEngine::CreateMesh("assets/tree.sdkmesh.colony");
        renderParams.texture = RenderEngine::CreateTexture("assets/treediffuse.png");
        int renderComponent = _renderComponent->CreateComponent(&renderParams);
        _entitySystem->AttachComponent(treeEntity, _renderComponent, renderComponent);
    }
}

void Frame(void)
{
    float elapsedTime = timerGetDeltaTime(&_timer);

    // Read
    _renderComponent->Read();
    _physicsComponent->Read();
    _firstPersonComponent->Read();
    _cameraComponent->Read();
    // Update
    _renderComponent->Update(elapsedTime);
    _physicsComponent->Update(elapsedTime);
    _firstPersonComponent->Update(elapsedTime);
    _cameraComponent->Update(elapsedTime);
    _digComponent->Update(elapsedTime);
    // Write
    _renderComponent->Write();
    _physicsComponent->Write();
    _firstPersonComponent->Write();
    _cameraComponent->Write();
    
    static int frameCount = 0;
    static float frameTime = 0.0f;
    frameCount++;
    frameTime+= elapsedTime;
    if(frameTime > 1.0f)
    {
        printf("FPS: %d\n", frameCount);
        frameCount = 0;
        frameTime -= 1.0f;
    }

    if(System::GetKeyState(System::Key::kG))
    {
        struct vert
        {
            float pos[3];
            float norm[3];
            float tex[2];
        };
    
        vert* verts = (vert*)terrainVertices;
        for(int ii=0; ii<terrainSize*terrainSize; ++ii)
        {
            verts[ii].pos[1] = _terrainHeights[ii] = -terrainHeight+1;
        }
        RenderEngine::UpdateMeshData(terrainMesh, verts);
    }
}

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
int main(int, char*[])
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_16_DF );
#endif
    cJSON* coreJson = cJSON_Parse(kEngineJson);
    engine_params_t params(coreJson);


    Core core;
    core.Init(coreJson);
    cJSON_Delete(coreJson);

    Initialize();
    while(1)
    {
        Frame();

        if(core.Frame())
            break;
    }

    core.Shutdown();

    delete _renderComponent;
    delete _entitySystem;
    delete _physicsComponent;
    delete _cameraComponent;
    delete _firstPersonComponent;
    delete _terrainHeights;
    delete _digComponent;

    if(terrainVertices)
        delete [] terrainVertices;

    return 0;
}
