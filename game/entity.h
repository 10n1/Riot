/*
 * entity.h
 * Riot
 *
 * Created by Kyle Weicht on 2/1/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __Riot_entity_h__
#define __Riot_entity_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "vm.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
class Entity
{
/* Methods */
public:
    static Entity* CreateEntity(void);

    const Transform& GetTransform(void);

/* Members */
private:
    static Entity*  s_entities;
    static int      s_numEntities;

    Transform   _transform;
    int         _id;

};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
