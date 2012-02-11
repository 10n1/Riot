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
Entity  _ground;
timer_t _timer;
int     _activeBricks = 0;
Entity  _camera;

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
    //_background.AddComponent(render);

    render = new RenderComponent();
    render->_mesh = RenderEngine::CreateMesh("assets/QuadMesh.json");
    render->_texture = RenderEngine::CreateTexture("assets/brick.png");
    render->_worldView = 1;
    _bricks[0].AddComponent(render);

    // Create bricks
    int brickIndex = 0;
    int towerWidth = 32;
    float startX = -63.0f;
    float y = 0.5f;
    while(towerWidth)
    {
        int ii = 0;
        float x = startX;
        for(; ii < towerWidth; ++ii, x += 2.0f)
        {
            // Physics
            {
#if 0
                //PhysicsComponent* physics = new PhysicsComponent();
                //b2BodyDef bodyDef;
                //b2FixtureDef fixtureDef;
                //b2PolygonShape dynamicBox;
                //
                //bodyDef.type = b2_dynamicBody;
                //bodyDef.position.Set(x, y);
                //physics->_physicsBody = PhysicsComponent::_world->CreateBody(&bodyDef);
                //
                //dynamicBox.SetAsBox(1.0f, 0.5f);
                //fixtureDef.shape = &dynamicBox;
                //fixtureDef.density = 1.0f;
                //fixtureDef.friction = 0.65f;
                //physics->_physicsBody->CreateFixture(&fixtureDef);
                //_bricks[brickIndex].AddComponent(physics);
#endif
                float omar = 0.5f;
                btCollisionShape* colShape = new btBoxShape(btVector3(omar, omar, omar));

                /// Create Dynamic Objects
                btTransform startTransform;
                startTransform.setIdentity();

                btScalar	mass(1.f);

                //rigidbody is dynamic if and only if mass is non zero, otherwise static
                bool isDynamic = (mass != 0.f);

                btVector3 localInertia(0,0,0);
                if (isDynamic)
                    colShape->calculateLocalInertia(mass,localInertia);

                startTransform.setOrigin(btVector3(x,y,0));

                //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
                btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
                btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
                PhysicsComponent* physics = new PhysicsComponent();
                physics->_body = new btRigidBody(rbInfo);

                PhysicsComponent::_dynamicsWorld->addRigidBody(physics->_body);
                _bricks[brickIndex].AddComponent(physics);
            }

            // Render
            render = new RenderComponent();
            render->_mesh = RenderEngine::CreateMesh("assets/cubemesh.json");
            //render->_mesh = RenderEngine::CreateMesh("assets/factory.sdkmesh.colony");
            //render->_texture = RenderEngine::CreateTexture("assets/treediffuse.png");
            render->_texture = RenderEngine::CreateTexture("assets/factorydiffuse.png");
            render->_worldView = 1;
            //if(brickIndex == 30)
                _bricks[brickIndex].AddComponent(render);
            _bricks[brickIndex]._transform.position.x = x;
            _bricks[brickIndex]._transform.position.y = y;
            brickIndex++;
            _activeBricks++;

            if(brickIndex == 50)
            {
                //FirstPersonController* fps = new FirstPersonController();
                //fps->_cameraSpeed = 10.0f;
                //fps->_lookSpeed = 1.0f;
                //_bricks[brickIndex].AddComponent(fps);
            }
        }
        y += 1.0f;
        startX += 1.0f;
        --towerWidth;
    }

    //_bricks[brickIndex-5].AddComponent(new CameraComponent);
    // Create plane
    render = new RenderComponent();
    render->_mesh = RenderEngine::CreateMesh("assets/quadmesh.json");
    render->_texture = RenderEngine::CreateTexture("assets/grass.jpg");
    render->_worldView = 1;
    _ground.AddComponent(render);
    _ground._transform.orientation = QuatRotationX(DegToRad(90.0f));
    _ground._transform.scale = 100.0f;

    CameraComponent* camComp = new CameraComponent();
    _camera.AddComponent(camComp);
    _camera._transform.position.y += 10.0f;
    _camera._transform.position.z -= 100.0f;
    FirstPersonController* fps = new FirstPersonController();
    fps->_cameraSpeed = 10.0f;
    fps->_lookSpeed = 1.0f;
    _camera.AddComponent(fps);
}

void Frame(void)
{
    float elapsedTime = timerGetDeltaTime(&_timer);
    _background.Update(elapsedTime);

    PhysicsComponent::_dynamicsWorld->stepSimulation(elapsedTime, 10);

    for(int ii=0; ii<_activeBricks; ++ii)
    {
        _bricks[ii].Update(elapsedTime);
    }
    _ground.Update(elapsedTime);
    _camera.Update(elapsedTime);
    
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
