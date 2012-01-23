/*
 * entity.cpp
 * PROJECTNAME
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "entity.h"

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "vm.h"

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
constant_buffer_t*   Entity::s_constBuffer  = NULL;
graphics_t*          Entity::s_graphics     = NULL;

/*******************************************************************\
External functions
\*******************************************************************/
/* Members */
void Entity::SetGraphics(graphics_t* graphics) { s_graphics = graphics; }
void Entity::SetConstantBuffer(constant_buffer_t* constBuffer) { s_constBuffer = constBuffer; }
void Entity::CreateEntity(  Entity* entity, b2World* world, 
                            texture_t* texture, mesh_t* mesh,
                            float x, float y, 
                            float w, float h, 
                            float density, float friction)
{
    // Graphics components
    entity->_texture = texture;
    entity->_mesh = mesh;

    // Physics components
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2PolygonShape dynamicBox;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    entity->_physicsBody = world->CreateBody(&bodyDef);

    dynamicBox.SetAsBox(w/2, h/2);
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    entity->_physicsBody->CreateFixture(&fixtureDef);
}
void Entity::Update(float elapsedTime)
{
}
void Entity::Render(void)
{
    Matrix4 worldMatrix = Matrix4RotationZ(_physicsBody->GetAngle());
    worldMatrix = Matrix4MatrixMultiply(Matrix4Scale(2.0f, 1.0f, 1.0f), worldMatrix);
    b2Vec2 pos = _physicsBody->GetPosition();
    worldMatrix.r3.x = pos.x;
    worldMatrix.r3.y = pos.y;
    gfxUpdateConstantBuffer(s_graphics, s_constBuffer, sizeof(worldMatrix), &worldMatrix);

    gfxSetTexture(s_graphics, _texture);
    gfxDrawMesh(s_graphics, _mesh);
}
