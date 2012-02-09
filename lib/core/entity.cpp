/*
 * entity.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/5/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "entity.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "component.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/
static int  s_entityId = 0;

/*******************************************************************\
Internal functions
\*******************************************************************/

} // anonymous namespace

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
Entity::Entity()
    : _transform(TransformZero())
    , _id(++s_entityId)
{
}
Entity::~Entity()
{
    for(int ii=0; ii<_numComponents; ++ii)
        delete _components[ii];
}
void Entity::AddComponent(Component* component)
{
    component->_entity = this;
    _components[_numComponents++] = component;
}
void Entity::Update(void)
{
    for(int ii=0; ii<_numComponents; ++ii)
    {
        _components[ii]->Update();
    }
}
