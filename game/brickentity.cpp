/*
 * BrickEntity.cpp
 * PROJECTNAME
 *
 * Created by Kyle Weicht on 1/22/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "BrickEntity.h"

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

/*******************************************************************\
External functions
\*******************************************************************/
/* Members */
void BrickEntity::CreateBrickEntity(  BrickEntity* BrickEntity, b2World* world,
                            texture_id_t texture, mesh_id_t mesh,
                            float x, float y, 
                            float w, float h, 
                            float density, float friction)
{
    // Graphics components
    BrickEntity->_texture = texture;
    BrickEntity->_mesh = mesh;

    // Physics components
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2PolygonShape dynamicBox;

    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(x, y);
    BrickEntity->_physicsBody = world->CreateBody(&bodyDef);

    dynamicBox.SetAsBox(w/2, h/2);
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    BrickEntity->_physicsBody->CreateFixture(&fixtureDef);

    BrickEntity->_width = w;
    BrickEntity->_height = h;
}
void BrickEntity::Update(float elapsedTime)
{
}
void BrickEntity::Render(void)
{
    Matrix4 worldMatrix = Matrix4RotationZ(_physicsBody->GetAngle());
    worldMatrix = Matrix4MatrixMultiply(Matrix4Scale(_width, _height, 1.0f), worldMatrix);
    b2Vec2 pos = _physicsBody->GetPosition();
    worldMatrix.r3.x = pos.x;
    worldMatrix.r3.y = pos.y;

    renderSubmitDraw(1, 0, _texture, &worldMatrix.r0.x, _mesh);

}
