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

    void GenerateNewTerrain(void);
    void MouseClick(int x, int y);

    static const int kWorldSize = 1024;

private:
    enum tixel_type_e
    {
        kDirt,
        kGrass,
        kSky,

        kNothing,
    };
    struct tixel_t
    {
        tixel_type_e    type;
    };

    void Circle(int x0, int y0, int radius, tixel_type_e type, int fill);

/* Members */
private:
    uint8_t*            _terrainTextureData;
    Render::texture_t   _terrainTexture;
    Render::material_t  _terrainMaterial;
    Render::mesh_t      _terrainMesh;
    
    Render::texture_t   _skyTexture;
    Render::material_t  _skyMaterial;
    Render::mesh_t      _skyMesh;    

    tixel_t _worldData[kWorldSize][kWorldSize];
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
