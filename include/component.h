/*
 * component.h
 * RiotLib
 *
 * Created by Kyle Weicht on 2/5/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __RiotLib_component_h__
#define __RiotLib_component_h__

#define PHYSICS_ENABLED 1

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "renderEngine.h"
#if PHYSICS_ENABLED
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#endif

/*******************************************************************\
External Constants And types
\*******************************************************************/
class Entity;

enum { MAX_ENTITIES = 1024*16 };

enum ComponentType
{
    TypeRender,
    TypePhysics,
    TypeFirstPerson,
    TypeCamera,

    TypeNull = -1
};

struct ComponentParams
{
};

class Component
{
/* Methods */
public:
    Component() : numActive(0) { }
    virtual ~Component() { }
    virtual int CreateComponent(ComponentParams* params)
    {
        return numActive++;
    }
    virtual void Read(void) { }
    virtual void Update(float elapsedTime) { }
    virtual void Write(void) { }
    void AttachToEntity(int index, Entity* entity)
    {
        entities[index] = entity;
    }
    virtual ComponentType Type(void) { return TypeNull; }

/* Members */
protected:
    Entity* entities[MAX_ENTITIES];
    int     numActive;
};

/*--------------------Render Component----------------*/
struct RenderComponentParams : public ComponentParams
{
    mesh_id_t       mesh;
    texture_id_t    texture;
    int             worldView;
};
class RenderComponent : public Component
{
/* Methods */
public:
    int CreateComponent(ComponentParams* params)
    {
        int index = Component::CreateComponent(params);

        RenderComponentParams* renderParams = (RenderComponentParams*)params;
        mesh[index] = renderParams->mesh;
        texture[index] = renderParams->texture;
        worldView[index] = renderParams->worldView;

        return index;
    }
    void Update(float elapsedTime);
    virtual ComponentType Type(void) { return TypeRender; }

/* Members */
protected:
    mesh_id_t       mesh[MAX_ENTITIES];
    texture_id_t    texture[MAX_ENTITIES];
    int             worldView[MAX_ENTITIES];
};

/*--------------------Physics Component----------------*/
struct PhysicsComponentParams : public ComponentParams
{
    Transform transform;
    float mass;
    float halfWidth;
    float halfHeight;
    float halflength;
};

extern float* _terrainHeights;
extern const int terrainSize;

class PhysicsComponent : public Component
{
/* Methods */
public:
    PhysicsComponent()
    {
        #if PHYSICS_ENABLED
        ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
        _collisionConfiguration = new btDefaultCollisionConfiguration();
        
        ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
        _dispatcher = new	btCollisionDispatcher(_collisionConfiguration);
        
        ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
        _overlappingPairCache = new btDbvtBroadphase();
        
        ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
        _solver = new btSequentialImpulseConstraintSolver;
        
        _dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher,_overlappingPairCache,_solver,_collisionConfiguration);

        _dynamicsWorld->setGravity(btVector3(0,-10,0));

        /****************/
        {
#if 0
            btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(500.),btScalar(1.0f),btScalar(500.)));
            btTransform groundTransform;
            groundTransform.setIdentity();
            groundTransform.setOrigin(btVector3(0, -1.0f, 0.0f));

            btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, groundShape, btVector3(0,0,0));
            btRigidBody* body = new btRigidBody(rbInfo);

            _dynamicsWorld->addRigidBody(body);
#else
            btCollisionShape* groundShape = new btHeightfieldTerrainShape(terrainSize, terrainSize, _terrainHeights, 1.0f, -100.0f, 100.0f, 1, PHY_FLOAT, false);
            
            btTransform groundTransform;
            groundTransform.setIdentity();
            groundTransform.setOrigin(btVector3(terrainSize/2, 0.0f, terrainSize/2));

            btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, groundShape, btVector3(0,0,0));
            rbInfo.m_friction = 1.0f;
            btRigidBody* body = new btRigidBody(rbInfo);
            _dynamicsWorld->addRigidBody(body);
#endif
        }
#endif
    }
    int CreateComponent(ComponentParams* params)
    {
        int index = Component::CreateComponent(params);

        PhysicsComponentParams* physicsParams = (PhysicsComponentParams*)params;

#if PHYSICS_ENABLED
        btTransform startTransform;
        startTransform.setIdentity();

        btCollisionShape* colShape = new btBoxShape(btVector3(physicsParams->halfWidth, physicsParams->halfHeight, physicsParams->halflength));;
        btScalar	mass(physicsParams->mass);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            colShape->calculateLocalInertia(mass,localInertia);

        startTransform.setOrigin(btVector3(physicsParams->transform.position.x, physicsParams->transform.position.y, physicsParams->transform.position.z));

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);

        bodies[index] = new btRigidBody(rbInfo);
        _dynamicsWorld->addRigidBody(bodies[index]);
#else
        transforms[index] = physicsParams->transform;
#endif
        return index;
    }
    void Update(float elapsedTime);
    void Write(void);
    virtual ComponentType Type(void) { return TypePhysics; }

/* Members */
protected:
#if PHYSICS_ENABLED
    btDefaultCollisionConfiguration*    _collisionConfiguration;
    btCollisionDispatcher*              _dispatcher;
    btBroadphaseInterface*              _overlappingPairCache;
    btSequentialImpulseConstraintSolver*_solver;
    btDiscreteDynamicsWorld*            _dynamicsWorld;

    btRigidBody* bodies[MAX_ENTITIES];
#else
    Transform transforms[MAX_ENTITIES];
#endif
    
};



/*--------------------FirstPerson Component----------------*/
class FirstPersonComponent : public Component
{
/* Methods */
public:
    int CreateComponent(ComponentParams* params)
    {
        return Component::CreateComponent(params);
    }
    void Update(float elapsedTime);
    void Write(void);
    virtual ComponentType Type(void) { return TypeFirstPerson; }

/* Members */
protected:
    Transform newTransforms[MAX_ENTITIES];
};

/*--------------------Camera Component----------------*/
class CameraComponent : public Component
{
/* Methods */
public:
    int CreateComponent(ComponentParams* params)
    {
        return Component::CreateComponent(params);
    }
    void Update(float elapsedTime);
    virtual ComponentType Type(void) { return TypeCamera; }

/* Members */
protected:
};

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/

#endif /* include guard */
