/*
 * BrickEntity.h
 * Riot
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __Riot_BrickEntity_h__
#define __Riot_BrickEntity_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "renderEngine.h"
#include "Box2D/Box2D.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/

class BrickEntity
{
/* Methods */
public:
    void Update(float elapsedTime);
    void Render(void);

    static void CreateBrickEntity(   BrickEntity* BrickEntity, b2World* world,
                                texture_id_t texture, mesh_id_t mesh,
                                float x, float y, 
                                float w, float h, 
                                float density, float friction);

/* Members */
private:
    texture_id_t  _texture;
    mesh_id_t     _mesh;

    b2Body*     _physicsBody;
    float       _width;
    float       _height;
};


/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
