/*
 * world.h
 * game
 *
 * Created by Kyle Weicht on 1/14/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __game_world_h__
#define __game_world_h__

/* C headers */
#include <stdint.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "render_engine/renderEngine.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
class World
{
/* Methods */
public:
    void Create(void);
    void Destroy(void);

    void UpdateTerrainTexture(void);

    void Update(float elapsedTime);
    void Render(void);

    static const int kWorldSize = 1024;

private:
    

/* Members */
private:
    uint8_t*            _terrainTextureData;
    Render::texture_t   _terrainTexture;
    Render::material_t  _terrainMaterial;
    Render::mesh_t      _terrainMesh;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
