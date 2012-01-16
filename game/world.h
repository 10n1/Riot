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
#include "player.h"
#include "materials.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
class World
{
friend class Player;
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

    struct tixel_t
    {
        material_type_e type;
        float           durability;
    };

    void CircleFill(int x0, int y0, int radius, material_type_e type, int fill);
    void CircleFunc(int x0, int y0, int radius, int fill, void (*func)(tixel_t*,void*), void* param);
    const tixel_t* GetTixel(int x, int y);

    float CalculateSlope(int x, int y);

    int CalculateNearestHeight(int x, int y);

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

    Player  _player;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
