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
/* Internal headers */
#include "global.h"
#include "core.h"
#include "renderEngine.h"
#include "entity.h"
#include "component.h"

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
Entity  _bricks[1024*8];

/*******************************************************************\
Internal functions
\*******************************************************************/
void Initialize(void)
{
    RenderComponent* render = new RenderComponent(&_background);
    render->_mesh = RenderEngine::CreateMesh("assets/quadMesh.json");
    render->_texture = RenderEngine::CreateTexture("assets/ground.png");
    render->_worldView = 0;
    _background.AddComponent(render);

    render = new RenderComponent(&_bricks[0]);
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
            render = new RenderComponent(&_bricks[brickIndex]);
            render->_mesh = RenderEngine::CreateMesh("assets/brickMesh.json");
            render->_texture = RenderEngine::CreateTexture("assets/brick.png");
            render->_worldView = 1;
            _bricks[brickIndex].AddComponent(render);
            Transform t = { QuaternionZero(), { x, y, 0.0f } };
            _bricks[brickIndex].SetTransform(t);

            brickIndex++;
        }
        y += 1.0f;
        startX += 1.0f;
        --towerWidth;
    }
}

void Frame(void)
{
    _background.Update();

    for(int ii=0; ii<ARRAY_LENGTH(_bricks); ++ii)
    {
        _bricks[ii].Update();
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

    return 0;
}
