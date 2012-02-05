/*
 * component.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/5/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_component_h__
#define __RiotLib_component_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "renderEngine.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
class Entity;

class Component
{
/* Methods */
public:
    Component(Entity* entity) : _entity(entity) { }
    virtual ~Component() { }
    virtual void Update(void) = 0;

/* Members */
protected:
    Entity* _entity;
};

class RenderComponent : public Component
{
/* Methods */
public:
    RenderComponent(Entity* entity) : Component(entity) {}
    void Update(void);

/* Members */
public:
    mesh_id_t       _mesh;
    texture_id_t    _texture;
    int             _worldView;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
