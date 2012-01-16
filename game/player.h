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
#include "materials.h"
#include "render_engine/renderEngine.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
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

    void AddToInventory(material_type_e material, int count);
private:

/* Members */
private:
    World*              _world;

    int _inventory[128];
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
