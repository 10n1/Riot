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
#include "Box2D/Box2D.h"
#include "entity.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
enum
{
#if defined(DEBUG) || defined(_DEBUG) 
    kMaxEntities = 512,
#else   
    kMaxEntities = 2048,
#endif
};

class World
{
/* Methods */
public:
    World(void);

    void Create(void);
    void Destroy(void);

    void Update(float elapsedTime);
    void Render(void);

    void SetGraphicsDevice(graphics_t* graphics);

    void BuildBuilding(void);
    void Reset(void);

    void Explosion(float x, float y, float radius, float force);

    void ConvertToWorldPos(float* x, float* y);

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
    Entity      _activeEntities[kMaxEntities];
    int         _numActiveEntities;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
