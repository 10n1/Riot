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
#include "Box2D/Box2D.h"

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
b2World* PhysicsComponent::_world = NULL;

/*******************************************************************\
External functions
\*******************************************************************/
void RenderComponent::Update(void)
{
    RenderEngine::Render(_worldView, TransformGetMatrix(_entity->transform()), _mesh, _texture);
}
void PhysicsComponent::Initialize(void)
{
    // World
    _world = new b2World(b2Vec2(0.0f, -9.8f));
    _world->SetAllowSleeping(true);

    // Ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -4.0f);
    b2Body* groundBody = _world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(128.0f, 4.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
}
void PhysicsComponent::Update(void)
{
    Matrix4 worldMatrix = Matrix4RotationZ(_physicsBody->GetAngle());

    b2Vec2 pos = _physicsBody->GetPosition();
    Quaternion q = QuatRotationZ(_physicsBody->GetAngle());

    Transform t = { q, {pos.x, pos.y, 0.0f} };

    _entity->SetTransform(t);
}