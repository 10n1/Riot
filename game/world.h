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
#include "renderEngine.h"
#include "Box2D/Box2D.h"
#include "BrickEntity.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
enum
{
#if defined(DEBUG) || defined(_DEBUG) 
    kMaxEntities = 512
#else   
    kMaxEntities = 4096
#endif
};

enum material_type_e
{
    kBrick,
    kWood,

    kNUM_MATERIALS
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

    void BuildBuilding(void);
    void Reset(void);

    void Explosion(float x, float y, float radius, float force);

    void ConvertToWorldPos(float* x, float* y);
    void Resize(float width, float height);

/* Members */
private:
    mesh_id_t     _quadMesh;
    texture_id_t  _backgroundTexture;
    texture_id_t  _brickTexture;
    texture_id_t  _woodTexture;
    material_id_t _material;
    b2World*    _box2d;
    BrickEntity      _activeEntities[kMaxEntities];
    int         _numActiveEntities;
};

struct material_properties_t
{
    float density;
    float friction;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
