/*
 * camera.h
 * graphicsDevice
 *
 * Created by Kyle Weicht on 1/21/2012.
 * Copyright (c) 2012 Kyle Weicht. All rights reserved.
 */
#ifndef __graphicsDevice_camera_h__
#define __graphicsDevice_camera_h__

/* C headers */
/* C++ headers */
/* External headers */
/* Internal headers */
#include "vm.h"

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*******************************************************************\
External Constants And types
\*******************************************************************/
#if 0
typedef struct camera_t
{
    Matrix4 view;
    
    Vector3 position;
    Vector4 up;
    Vector4 look;
    Vector4 right;

    Quaternion  orientation;
} camera_t;
#else
typedef Transform camera_t;
#endif

/*******************************************************************\
External variables
\*******************************************************************/

/*******************************************************************\
External functions
\*******************************************************************/
void camInit(camera_t* cam);

void camTranslateX(camera_t* cam, float t);
void camTranslateY(camera_t* cam, float t);
void camTranslateZ(camera_t* cam, float t);

void camRotateX(camera_t* cam, float r);
void camRotateY(camera_t* cam, float r);

Matrix4 camGetViewMatrix(camera_t* cam);


#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif /* include guard */
