/*
 * component.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/5/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "component.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "entity.h"
#include "system.h"

namespace
{

/*******************************************************************\
Internal Constants And types
\*******************************************************************/

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
btDefaultCollisionConfiguration*    PhysicsComponent::_collisionConfiguration;
btCollisionDispatcher*              PhysicsComponent::_dispatcher;
btBroadphaseInterface*              PhysicsComponent::_overlappingPairCache;
btSequentialImpulseConstraintSolver*PhysicsComponent::_solver;
btDiscreteDynamicsWorld*            PhysicsComponent::_dynamicsWorld;

/*******************************************************************\
External functions
\*******************************************************************/
void RenderComponent::Update(float elapsedTime)
{
    RenderEngine::Render(_worldView, TransformGetMatrix(_entity->transform()), _mesh, _texture);
}
void PhysicsComponent::Initialize(void)
{
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
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(500.),btScalar(1.0f),btScalar(500.)));
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -1.0f, 0.0f));

        btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, groundShape, btVector3(0,0,0));
        btRigidBody* body = new btRigidBody(rbInfo);

        _dynamicsWorld->addRigidBody(body);
    }
}
void PhysicsComponent::Update(float elapsedTime)
{
    btTransform bulletTransform;
    _body->getMotionState()->getWorldTransform(bulletTransform);

    Transform t;
    t.orientation = Vector4FromArray(bulletTransform.getRotation());
    t.position = Vector3FromArray(bulletTransform.getOrigin());
    t.scale = 1.0f;

    _entity->SetTransform(t);
}
void CameraComponent::Update(float elapsedTime)
{
    Matrix4 viewMatrix;
    Matrix3 temp;
    Vector3 x, y, z;

    const Transform& transform = _entity->transform();
    temp = QuaternionGetMatrix(transform.orientation);
    viewMatrix = Matrix4FromMatrix3(temp);

    temp = Matrix3Transpose(temp);
    x = temp.r0;
    y = temp.r1;
    z = temp.r2;

    viewMatrix.r3.x = -Vector3DotProduct(x, transform.position);
    viewMatrix.r3.y = -Vector3DotProduct(y, transform.position);
    viewMatrix.r3.z = -Vector3DotProduct(z, transform.position);

    RenderEngine::SetWorldViewMatrix(viewMatrix);
}
void FirstPersonController::Update(float elapsedTime)
{
    int mouseX;
    int mouseY;
    System::GetMousePosition(&mouseX, &mouseY);
    int deltaX = 0; // mouseX - _mouseX;
    int deltaY = 0; // mouseY - _mouseY;
    _mouseX = mouseX;
    _mouseY = mouseY;

    float speed = elapsedTime * _cameraSpeed;
    if(System::GetKeyState(System::Key::kShift))
        speed *= 3.0f;
    float lookSpeed = elapsedTime * _lookSpeed;
    if(System::GetKeyState(System::Key::kUp) || deltaY < 0)
        TransformRotateX(&_entity->_transform, -lookSpeed);
    if(System::GetKeyState(System::Key::kDown) || deltaY > 0)
        TransformRotateX(&_entity->_transform, +lookSpeed);
    if(System::GetKeyState(System::Key::kLeft) || deltaX < 0)
        TransformRotateY(&_entity->_transform, -lookSpeed);
    if(System::GetKeyState(System::Key::kRight) || deltaX > 0)
        TransformRotateY(&_entity->_transform, +lookSpeed);

    if(System::GetKeyState(System::Key::kW))
        TransformTranslateZ(&_entity->_transform, speed);
    if(System::GetKeyState(System::Key::kS))
        TransformTranslateZ(&_entity->_transform, -speed);
        
    if(System::GetKeyState(System::Key::kA))
        TransformTranslateX(&_entity->_transform, -speed);
    if(System::GetKeyState(System::Key::kD))
        TransformTranslateX(&_entity->_transform, +speed);
        
    if(System::GetKeyState(System::Key::kSpace))
        TransformTranslateY(&_entity->_transform, +speed);
    if(System::GetKeyState(System::Key::kC))
        TransformTranslateY(&_entity->_transform, -speed);
}
