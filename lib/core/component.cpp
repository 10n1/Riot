/*
 * component.cpp
 * RiotLib
 *
 * Created by Kyle Weicht on 2/5/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "component.h"

/* C headers */
#include <stdio.h>
#include <Windows.h>
/* C++ headers */
/* External headers */
/* Internal headers */
#include "entity.h"
#include "system.h"
#include "perlin.h"
#include "terrain.h"


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
Matrix4 _viewMatrix;
Matrix4 _projMatrix;

/*******************************************************************\
External functions
\*******************************************************************/
void RenderComponent::Update(float elapsedTime) 
{
    for(int ii=0; ii<numActive; ++ii)
    {
        RenderEngine::Render(worldView[ii], TransformGetMatrix(entities[ii]->transform), mesh[ii], texture[ii]);
    }
}

void PhysicsComponent::Update(float elapsedTime)
{
#if PHYSICS_ENABLED
    _dynamicsWorld->stepSimulation(elapsedTime, 10);
#endif
}
void PhysicsComponent::Write(void)
{
    for(int ii=0; ii<numActive; ++ii)
    {
#if PHYSICS_ENABLED
        btTransform bulletTransform;
        bodies[ii]->getMotionState()->getWorldTransform(bulletTransform);
        Transform t;
        t.orientation = Vector4FromArray(bulletTransform.getRotation());
        t.position = Vector3FromArray(bulletTransform.getOrigin());
        t.scale = 1.0f;
        entities[ii]->transform = t;
#else
        entities[ii]->transform = transforms[ii];
#endif
    }
}
extern Perlin perlin;

Vector3 CalculatePickRay(float x, float y)
{
    Vector4 coord;
    coord.x = ((2.0f*x)/1024)-1;
    coord.y = -(((2.0f*y)/1024)-1);
    coord.z = 1.0f;
    coord.w = 1.0f;

    coord.x /= _projMatrix.r0.x;
    coord.y /= _projMatrix.r1.y;
    
    Matrix4 viewInverse = Matrix4Inverse(_viewMatrix);

    coord = Vector4MulScalar(coord, 10000.0f);
    coord = Matrix4VectorMultiply(viewInverse, coord);

    return Vector3FromVector4(coord);
}
void FirstPersonComponent::Update(float elapsedTime)
{
    Transform& transform = newTransforms[0];
    transform = entities[0]->transform;

    int deltaX = 0;
    int deltaY = 0;
    System::GetMouseDelta(&deltaX, &deltaY);

    float speed = elapsedTime * 10.0f;
    if(System::GetKeyState(System::Key::kShift))
        speed *= 3.0f;

    float lookSpeed = elapsedTime * 3.0f;
    if(System::GetKeyState(System::Key::kUp) || deltaY < 0)
        TransformRotateX(&transform, -lookSpeed);
    if(System::GetKeyState(System::Key::kDown) || deltaY > 0)
        TransformRotateX(&transform, +lookSpeed);
    if(System::GetKeyState(System::Key::kLeft) || deltaX < 0)
        TransformRotateY(&transform, -lookSpeed);
    if(System::GetKeyState(System::Key::kRight) || deltaX > 0)
        TransformRotateY(&transform, +lookSpeed);

    if(System::GetKeyState(System::Key::kW))
        TransformTranslateZ(&transform, speed);
    if(System::GetKeyState(System::Key::kS))
        TransformTranslateZ(&transform, -speed);
        
    if(System::GetKeyState(System::Key::kA))
        TransformTranslateX(&transform, -speed);
    if(System::GetKeyState(System::Key::kD))
        TransformTranslateX(&transform, +speed);

    if(System::GetKeyState(System::Key::kSpace))
        TransformTranslateY(&transform, +speed);
    if(System::GetKeyState(System::Key::kC))
        TransformTranslateY(&transform, -speed);
        
    if(System::GetKeyState(System::Key::kF))
        flying[0] = !flying[0];

    if(flying[0] == 0 && transform.position.x > 0.0f && transform.position.x < terrainSize
       && transform.position.z > 0.0f && transform.position.z < terrainSize)
    {
        transform.position.y = perlin.Get(transform.position.x/terrainSize,transform.position.z/terrainSize) + 2.0f;
    }

    if(System::GetMouseButtonState(System::Mouse::kLeft))
    {
        Vector3 ray = CalculatePickRay(512.0f, 768.0f/2);

        char buffer[256];
        sprintf(buffer, "X: %f Y: %f Z: %f\n", ray.x, ray.y, ray.z);
        OutputDebugString(buffer);
    }
}
void FirstPersonComponent::Write(void)
{
    entities[0]->transform = newTransforms[0];
}

void CameraComponent::Update(float elapsedTime)
{
    Matrix4 viewMatrix;
    Matrix3 temp;
    Vector3 x, y, z;

    const Transform& transform = entities[0]->transform;
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

    _viewMatrix = viewMatrix;
}

extern int terrainMesh;
extern void* terrainVertices;

void DigComponent::Update(float elapsedTime)
{
    static int down = 0;
    if(System::GetKeyState(System::Key::kR))
    {
        if(down != 0)
            return;

        struct vert
        {
            float pos[3];
            float norm[3];
            float tex[2];
        };

        vert* vertices = (vert*)terrainVertices;

        float x = entities[0]->transform.position.x;
        float z = entities[0]->transform.position.z;

        int nearestVertX = x;
        int nearestVertZ = z;
        for(int xx=x-1.0f; xx <= x+1.0f; ++xx)
        {
            for(int zz=z-1.0f; zz <= z+1.0f; ++zz)
            {
                float height = TERRAIN_INDEX(xx, zz) - 1.0f;
                TERRAIN_INDEX(xx, zz) = vertices[xx+zz*terrainSize].pos[1] = height;
            }
        }

        RenderEngine::UpdateMeshData(terrainMesh, vertices);
        down = 1;
    }
    else
        down = 0;
}
