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

Entity  _background;
Entity  _bricks[1024*4];
timer_t _timer;
int     _activeBricks = 0;

/*******************************************************************\
Internal functions
\*******************************************************************/
void Initialize(void)
{
    timerInit(&_timer);

    RenderComponent* render = new RenderComponent();
    render->_mesh = RenderEngine::CreateMesh("assets/quadMesh.json");
    render->_texture = RenderEngine::CreateTexture("assets/ground.png");
    render->_worldView = 0;
    _background.AddComponent(render);

    render = new RenderComponent();
    render->_mesh = RenderEngine::CreateMesh("assets/brickMesh.json");
    render->_texture = RenderEngine::CreateTexture("assets/brick.png");
    render->_worldView = 1;
    _bricks[0].AddComponent(render);

    // Create bricks
    int brickIndex = 0;
    int towerWidth = 64;
    float startX = -63.0f;
    float y = 16.5f;
    while(towerWidth)
    {
        int ii = 0;
        float x = startX;
        for(; ii < towerWidth; ++ii, x += 2.0f)
        {
            // Physics
            PhysicsComponent* physics = new PhysicsComponent();
            b2BodyDef bodyDef;
            b2FixtureDef fixtureDef;
            b2PolygonShape dynamicBox;

            bodyDef.type = b2_dynamicBody;
            bodyDef.position.Set(x, y);
            physics->_physicsBody = PhysicsComponent::_world->CreateBody(&bodyDef);

            dynamicBox.SetAsBox(1.0f, 0.5f);
            fixtureDef.shape = &dynamicBox;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.65f;
            physics->_physicsBody->CreateFixture(&fixtureDef);
            _bricks[brickIndex].AddComponent(physics);

            // Render
            render = new RenderComponent();
            render->_mesh = RenderEngine::CreateMesh("assets/brickMesh.json");
            render->_texture = RenderEngine::CreateTexture("assets/brick.png");
            render->_worldView = 1;
            _bricks[brickIndex].AddComponent(render);

            brickIndex++;
            _activeBricks++;
        }
        y += 1.0f;
        startX += 1.0f;
        --towerWidth;
    }
}

void Frame(void)
{
    _background.Update();
    float elapsedTime = timerGetDeltaTime(&_timer);

    PhysicsComponent::_world->Step(elapsedTime, 10, 3);

    for(int ii=0; ii<_activeBricks; ++ii)
    {
        _bricks[ii].Update();
    }
    
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

    PhysicsComponent::Shutdown();

    return 0;
}
