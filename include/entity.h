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
#include <string.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "vm.h"
#include "component.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
struct EntityDataNode
{
    ComponentType   type;
    int             index;
    EntityDataNode* next;
};

struct Entity
{
    Entity() : transform(TransformZero()), node(NULL) { }
    Transform       transform;
    EntityDataNode* node;
};

class EntitySystem
{
/* Methods */
public:
    EntitySystem() : numEntities(0), numNodes(0) { }
    ~EntitySystem() { }

    int CreateEntity(void) { return numEntities++; }
    Entity* GetEntity(int index) { return &entities[index]; }

    void AttachComponent(int entity, Component* component, int index)
    {
        Entity* e = &entities[entity];
        EntityDataNode* node = e->node;
        while(node && node->next)
            node = node->next;
            
        EntityDataNode* newNode = &nodes[numNodes++];
        newNode->type = component->Type();
        newNode->index = index;
        newNode->next = NULL;
        if(node)
            node->next = newNode;
        else
            e->node = newNode;

        component->AttachToEntity(index, GetEntity(entity));
    }
    int GetComponentIndex(int entity, ComponentType type)
    {
        EntityDataNode* node = entities[entity].node;
        while(node)
        {
            if(node->type == type)
                return node->index;
            node = node->next;
        }
        return -1;
    }

/* Members */
public:
    int             numEntities;
    int             numNodes;
    Entity          entities[MAX_ENTITIES];
    EntityDataNode  nodes[MAX_ENTITIES*8];
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
