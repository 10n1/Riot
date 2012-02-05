/*
 * camera.c
 * PROJECTNAME
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */

#include "camera.h"

/* C headers */
/* External headers */
/* Internal headers */


/*******************************************************************\
Internal Constants And types
\*******************************************************************/

/*******************************************************************\
Internal variables
\*******************************************************************/

/*******************************************************************\
Internal functions
\*******************************************************************/


/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void camInit(camera_t* cam)
{
    cam->position.x = 0.0f;
    cam->position.y = 0.0f;
    cam->position.z = 0.0f;
    cam->orientation = QuaternionZero();
}
void camTranslateX(camera_t* cam, float t)
{
    Vector3 x = QuaternionGetXAxis(cam->orientation);
    x = Vector3MulScalar(x, t);
    cam->position = Vector3Add(cam->position, x);
}
void camTranslateY(camera_t* cam, float t)
{
    Vector3 y = {0.0f, 1.0f, 0.0f};
    y = Vector3MulScalar(y, t);
    cam->position = Vector3Add(cam->position, y);
}
void camTranslateZ(camera_t* cam, float t)
{
    Vector3 z = QuaternionGetZAxis(cam->orientation);
    z = Vector3MulScalar(z, t);
    cam->position = Vector3Add(cam->position, z);
}

void camRotateX(camera_t* cam, float r)
{
    Vector3 xAxis = {1.0f, 0.0f, 0.0f};
    Quaternion q = QuatFromAxisAngle(xAxis, r);
    cam->orientation = QuaternionQuaternionMul(cam->orientation, q);
}
void camRotateY(camera_t* cam, float r)
{
    Vector3 yAxis = {0.0f, 1.0f, 0.0f};
    Quaternion q = QuatFromAxisAngle(yAxis, r);
    cam->orientation = QuaternionQuaternionMul(q, cam->orientation);
}

Matrix4 camGetViewMatrix(camera_t* cam)
{
    Matrix4 viewMatrix;
    Matrix3 temp;
    Vector3 x, y, z;

    temp = QuaternionGetMatrix(cam->orientation);
    viewMatrix = Matrix4FromMatrix3(temp);

    temp = Matrix3Transpose(temp);
    x = temp.r0;
    y = temp.r1;
    z = temp.r2;

    viewMatrix.r3.x = -Vector3DotProduct(x, cam->position);
    viewMatrix.r3.y = -Vector3DotProduct(y, cam->position);
    viewMatrix.r3.z = -Vector3DotProduct(z, cam->position);

    return viewMatrix;
}
