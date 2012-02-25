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

float* _terrainHeights = NULL;
const int terrainSize = 256;
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

/*******************************************************************\
Internal functions
\*******************************************************************/
void Initialize(void)
{
    timerInit(&_timer);
    _terrainHeights = GenerateTerrain(terrainSize);

    _entitySystem = new EntitySystem();
    _renderComponent = new RenderComponent();
    _physicsComponent = new PhysicsComponent();
    _cameraComponent = new CameraComponent();
    _firstPersonComponent = new FirstPersonComponent();


    /* Create background object */
    //int backgroundEntity = _entitySystem->CreateEntity();
    RenderComponentParams renderParams;
    renderParams.mesh =GenerateTerrainMesh(_terrainHeights, terrainSize);// RenderEngine::CreateMesh("assets/quadMesh.json");
    renderParams.texture = 0;
    renderParams.worldView = 0;
    int renderComponent;// = _renderComponent->CreateComponent(&renderParams);
    
    //_entitySystem->AttachComponent(backgroundEntity, _renderComponent, renderComponent);

    // Create ground
    int groundEntity = _entitySystem->CreateEntity();
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
    _entitySystem->AttachComponent(cameraEntity, _cameraComponent, cameraComponent);
    _entitySystem->AttachComponent(cameraEntity, _firstPersonComponent, firstPersonComponent);
    Entity* entity = _entitySystem->GetEntity(cameraEntity);
    entity->transform.position.x = terrainSize/2;
    entity->transform.position.y = 50;
    entity->transform.position.z = terrainSize/2-50.0f;
    // Create bricks
    int brickIndex = 0;
    int towerWidth = 32;
    float startX = -63.0f;
    float y = 0.5f;
    renderParams.mesh = RenderEngine::CreateMesh("assets/cubemesh.json");
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
            params.transform.position.y = y;
            params.transform.position.y += 100.0f;
            params.transform.position.x += terrainSize/2;
            params.transform.position.z += terrainSize/2;
            int physicsComponent = _physicsComponent->CreateComponent(&params);
            int renderComponent = _renderComponent->CreateComponent(&renderParams);
            _entitySystem->AttachComponent(brickEntity, _physicsComponent, physicsComponent);
            _entitySystem->AttachComponent(brickEntity, _renderComponent, renderComponent);
        }
        y += 1.0f;
        startX += 1.0f;
        --towerWidth;
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

    return 0;
}
