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
#include "btBulletDynamicsCommon.h"

/*******************************************************************\
External Constants And types
\*******************************************************************/
class Entity;

class Component
{
/* Methods */
public:
    virtual ~Component() { }
    virtual void Update(float elapsedTime) = 0;

/* Members */
protected:
    Entity* _entity;
    friend class Entity;
};

class RenderComponent : public Component
{
/* Methods */
public:
    void Update(float elapsedTime);

/* Members */
public:
    mesh_id_t       _mesh;
    texture_id_t    _texture;
    int             _worldView;
};
class PhysicsComponent : public Component
{
/* Methods */
public:

    static void Initialize(void);
    static void Shutdown(void) { }

    void Update(float elapsedTime);

/* Members */
public:
	static btDefaultCollisionConfiguration*    _collisionConfiguration;
	static btCollisionDispatcher*              _dispatcher;
	static btBroadphaseInterface*              _overlappingPairCache;
	static btSequentialImpulseConstraintSolver*_solver;
	static btDiscreteDynamicsWorld*            _dynamicsWorld;

    btRigidBody* _body;
};
class CameraComponent : public Component
{
/* Methods */
public:
    void Update(float elapsedTime);

/* Members */
public:
};

class FirstPersonController : public Component
{
/* Methods */
public:
    void Update(float elapsedTime);

/* Members */
public:
    float _cameraSpeed;
    float _lookSpeed;
    int   _mouseX;
    int   _mouseY;
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
