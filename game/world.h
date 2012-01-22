/*
 * world.h
 * Riot
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __Riot_world_h__
#define __Riot_world_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "graphicsDevice/graphicsDevice.h"
#include "Box2D\Box2D.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
enum
{
    kMaxObjects = 128,
};

class World
{
/* Methods */
public:
    void Create(void);
    void Destroy(void);

    void Update(float elapsedTime);
    void Render(void);

    void SetGraphicsDevice(graphics_t* graphics);

    void AddBrick(float x, float y);

/* Members */
private:
    graphics_t* _graphics;
    mesh_t*     _quadMesh;
    texture_t*  _backgroundTexture;
    texture_t*  _brickTexture;
    material_t* _material;
    constant_buffer_t*  _perFrameConstantBuffer;
    constant_buffer_t*  _perObjectConstantBuffer;
    b2World*    _box2d;
    b2Body*     _activeBodies[kMaxObjects];
    int         _numActiveBodies;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
