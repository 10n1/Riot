/*
 * entity.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/5/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_entity_h__
#define __RiotLib_entity_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "vm.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
class Component;

class Entity
{
/* Methods */
public:
    Entity();
    ~Entity();

    void AddComponent(Component* component);
    void Update(void);

    void SetTransform(const Transform& transform) { _transform = transform; }
    const Transform& transform() const { return _transform; }
    int id() const { return _id; }

/* Members */
public:
    Transform   _transform;
    int         _id;
    Component*  _components[128];
    int         _numComponents;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
