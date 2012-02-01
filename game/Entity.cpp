/*
 * Entity.cpp
 * Riot
 *
 * Created by Kyle Weicht on 2/1/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "Entity.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "global.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/
enum
{
    kMaxEntities = 1024*16,
};

/*******************************************************************\
Internal variables
\*******************************************************************/

/*******************************************************************\
Internal functions
\*******************************************************************/

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/
Entity*  Entity::s_entities     = NULL;
int      Entity::s_numEntities  = 0;


/*******************************************************************\
External functions
\*******************************************************************/
Entity* Entity::CreateEntity(void)
{
    if(s_entities == NULL)
    {
        s_entities = new Entity[kMaxEntities];
    }

    assert(s_numEntities < kMaxEntities);
    int entityIndex = s_numEntities++;

    Entity* entity = &s_entities[entityIndex];
    entity->_id = entityIndex;
    entity->_transform = TransformZero();
    return entity;
}

const Transform& Entity::GetTransform(void)
{
    return _transform;
}

