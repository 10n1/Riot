/*
 * player.h
 * Game
 *
 * Created by Kyle Weicht on 1/14/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __game_player_h__
#define __game_player_h__

/* C headers */
#include <stdint.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "render_engine/renderEngine.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
enum material_e
{
    kDirt,
    kGrass,

    kNothing,
};

class World;

class Player
{
friend class World;
/* Methods */
public:
    void Create(void);
    void Destroy(void);

    void Update(float elapsedTime);
    void Render(void);

    void AddToInventory(material_e material, int count);
private:
    void UpdatePlayerTexture(void);

/* Members */
private:
    uint8_t*            _playerTextureData;
    Render::texture_t   _playerTexture;
    Render::material_t  _playerMaterial;
    Render::mesh_t      _playerMesh;

    World*              _world;

    int _inventory[128];

    float _position[2];
    float _previousPosition[2];

    int _facingRight;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
