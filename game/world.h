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
struct graphics_t;

/*******************************************************************\
External Constants And types
\*******************************************************************/
class World
{
/* Methods */
public:
    void Create(void);
    void Destroy(void);

    void Update(float elapsedTime);
    void Render(void);

    void SetGraphicsDevice(graphics_t* graphics);

/* Members */
private:
    graphics_t* _graphics;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
