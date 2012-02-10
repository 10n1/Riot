/*
*  vm.h
*  math library
*
*  Created by Kyle Weicht on 11/9/11.
*  Copyright (c) 2011 Kyle Weicht. All rights reserved.
*/

#ifndef math_library_vm_h_
#define math_library_vm_h_

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4505) /* Unused function */
#endif

#include <stdint.h>
#include <math.h>

/*
Math Constants
*/
static const float kEpsilon  = 0.0001f;
static const float kPi       = 3.14159265f;
static const float k2Pi      = 3.14159265f*2.0f;
static const float kPiDiv2   = 3.14159265f/2.0f;
static const float kInvPi    = 1.0f/3.14159265f;
static const float kDegToRad = (3.14159265f/180.0f);
static const float kRadToDeg = (180.0f/3.14159265f);
static float DegToRad( float fDeg ) { return fDeg * kDegToRad; }
static float RadToDeg( float fRad ) { return fRad * kRadToDeg; }
static float Square( float f ) { return f*f; }
static float Abs(float f){ if(f < 0.0f) return -f; return f; }

#ifdef __cplusplus
    #define INLINE static inline
    
    #define VECTOR2_INPUT const Vector2&
    #define VECTOR3_INPUT const Vector3&
    #define VECTOR4_INPUT const Vector4&
    #define MATRIX3_INPUT const Matrix3&
    #define MATRIX4_INPUT const Matrix4&
    #define QUATERNION_INPUT const Quaternion&
    #define TRANSFORM_INPUT const Transform&
#else
    #define INLINE static
    
    #define VECTOR2_INPUT const Vector2
    #define VECTOR3_INPUT const Vector3
    #define VECTOR4_INPUT const Vector4
    #define MATRIX3_INPUT const Matrix3
    #define MATRIX4_INPUT const Matrix4
    #define QUATERNION_INPUT const Quaternion
    #define TRANSFORM_INPUT const Transform
#endif

INLINE void SwapFloat(float* a, float* b)
{
    float t = *a;
    *a = *b;
    *b = t;
}

#define MTX4_INDEX(f,r,c) ((f)[(r*4)+c])
#define MTX3_INDEX(f,r,c) ((f)[(r*3)+c])
/*
-------------------------------------------------------------------------------
Vector 2
-------------------------------------------------------------------------------
*/
typedef struct Vector2_t
{
    float x, y;
} Vector2;


/* */
INLINE Vector2 Vector2FromArray(const float* f)
{ 
    Vector2 ret = {f[0],f[1]}; 
    return ret; 
}

INLINE Vector2 Vector2Add(VECTOR2_INPUT l, VECTOR2_INPUT r) 
{ 
    Vector2 ret = {l.x+r.x, l.y+r.y}; 
    return ret; 
}
INLINE Vector2 Vector2Sub(VECTOR2_INPUT l, VECTOR2_INPUT r) 
{ 
    Vector2 ret = {l.x-r.x, l.y-r.y}; 
    return ret; 
}
INLINE int Vector2Equal(VECTOR2_INPUT l, VECTOR2_INPUT r) 
{ 
    return l.x == r.x && l.y == r.y; 
}
INLINE Vector2 Vector2Zero(void) 
{ 
    Vector2 ret = {0.0f, 0.0f};
    return ret; 
}
INLINE Vector2 Vector2Multiply(VECTOR2_INPUT l, VECTOR2_INPUT r)
{
    Vector2 ret = {l.x*r.x, l.y*r.y}; 
    return ret; 
}
INLINE float Vector2Hadd(VECTOR2_INPUT v) 
{
    return v.x+v.y; 
}
INLINE float Vector2Magnitude(VECTOR2_INPUT v)
{
    return sqrtf(v.x*v.x + v.y*v.y);
}

/*
-------------------------------------------------------------------------------
Vector 3
-------------------------------------------------------------------------------
*/
typedef struct Vector3_t
{
    float x, y, z;
} Vector3;


INLINE Vector3 Vector3FromArray(const float* f) 
{ 
    Vector3 ret = 
    {
        f[0],
        f[1],
        f[2]
    }; 
    return ret; 
}

INLINE Vector3 Vector3Add(VECTOR3_INPUT l, VECTOR3_INPUT r) 
{ 
    Vector3 ret = 
    {
        l.x+r.x, 
        l.y+r.y,
        l.z+r.z
    }; 
    return ret; 
}
INLINE Vector3 Vector3Sub(VECTOR3_INPUT l, VECTOR3_INPUT r) 
{
    Vector3 ret = 
    {
        l.x-r.x, 
        l.y-r.y, 
        l.z-r.z
    }; 
    return ret; 
}
INLINE Vector3 Vector3Negate(VECTOR3_INPUT v) 
{ 
    Vector3 ret =
    {
        -v.x,
        -v.y,
        -v.z
    }; 
    return ret; 
}
INLINE Vector3 Vector3MulScalar(VECTOR3_INPUT v, float f)
{ 
    Vector3 ret =
    {
        v.x*f,
        v.y*f,
        v.z*f
    }; 
    return ret; 
}
INLINE Vector3 Vector3DivScalar(VECTOR3_INPUT v, float f)
{ 
    Vector3 ret = 
    {
        v.x/f,
        v.y/f,
        v.z/f
    };
    return ret; 
}
INLINE Vector3 Vector3Multiply(VECTOR3_INPUT l, VECTOR3_INPUT r)
{ 
    Vector3 ret = 
    {
        l.x*r.x,
        l.y*r.y,
        l.z*r.z
    }; 
    return ret;
}
INLINE float Vector3Hadd(VECTOR3_INPUT v) 
{ 
    return v.x+v.y+v.z; 
}

INLINE int Vector3Equal(VECTOR3_INPUT l, VECTOR3_INPUT r) 
{ 
    return l.x == r.x && l.y == r.y && l.z == r.z; 
}
INLINE Vector3 Vector3Zero(void) 
{ 
    Vector3 ret = {0.0f, 0.0f, 0.0f}; 
    return ret; 
}

INLINE float Vector3DotProduct(VECTOR3_INPUT l, VECTOR3_INPUT r) 
{ 
    return l.x * r.x + l.y * r.y + l.z * r.z; 
}
INLINE Vector3 Vector3CrossProduct(VECTOR3_INPUT l, VECTOR3_INPUT r) 
{ 
    Vector3 ret = 
    {
        l.y*r.z-l.z*r.y,
        l.z*r.x-l.x*r.z,
        l.x*r.y-l.y*r.x
    };
    return ret; 
}
INLINE float Vector3Magnitude(VECTOR3_INPUT v) 
{ 
    return sqrtf(Vector3DotProduct(v,v));
}
INLINE float Vector3MagnitudeSq(VECTOR3_INPUT v) 
{ 
    return Vector3DotProduct(v, v); 
}
INLINE Vector3 Vector3Normalize(VECTOR3_INPUT v)
{ 
    return Vector3DivScalar(v, Vector3Magnitude(v));
}

/*
-------------------------------------------------------------------------------
Vector 4
-------------------------------------------------------------------------------
*/
typedef struct Vector4_t
{
    float x;
    float y;
    float z;
    float w;
} Vector4;

INLINE Vector4 Vector4FromArray(const float* f) 
{ 
    Vector4 ret = 
    {
        f[0],
        f[1],
        f[2],
        f[3]
    }; 
    return ret;
}
INLINE Vector4 Vector4FromVector3(VECTOR3_INPUT v, float f)
{
    Vector4 ret = 
    {
        v.x,
        v.y,
        v.z,
        f
    }; 
    return ret;
}
INLINE Vector4 Vector4Add(VECTOR4_INPUT l, VECTOR4_INPUT r) 
{ 
    Vector4 ret = 
    {
        l.x+r.x, 
        l.y+r.y, 
        l.z+r.z,
        l.w+r.w
    };
    return ret; 
}
INLINE Vector4 Vector4Sub(VECTOR4_INPUT l, VECTOR4_INPUT r) 
{
    Vector4 ret = 
    {
        l.x-r.x,
        l.y-r.y, 
        l.z-r.z,
        l.w-r.w
    }; 
    return ret; 
}
INLINE Vector4 Vector4Negate(VECTOR4_INPUT v)
{
    Vector4 ret = 
    {
        -v.x,
        -v.y,
        -v.z,
        -v.w
    }; 
    return ret; 
}
INLINE Vector4 Vector4MulScalar(VECTOR4_INPUT v, float f)
{ 
    Vector4 ret =
    {
        v.x*f,
        v.y*f,
        v.z*f,
        v.w*f
    };
    return ret; 
}
INLINE Vector4 Vector4DivScalar(VECTOR4_INPUT v, float f)
{
    float recip = 1.0f/f;
    Vector4 ret = 
    {
        v.x*recip,
        v.y*recip,
        v.z*recip,
        v.w*recip
    };
    return ret;
}
INLINE Vector4 Vector4Multiply(VECTOR4_INPUT l, VECTOR4_INPUT r) 
{ 
    Vector4 ret = 
    {
        l.x*r.x, 
        l.y*r.y, 
        l.z*r.z, 
        l.w*r.w
    };
    return ret; 
}

INLINE float Vector4Hadd(VECTOR4_INPUT v)
{ 
    return v.x+v.y+v.z+v.w; 
}

INLINE int Vector4Equal(VECTOR4_INPUT l, VECTOR4_INPUT r) 
{ 
    return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w; 
}
INLINE Vector4 Vector4Zero(void) 
{ 
    Vector4 ret =
    {
        0.0f, 
        0.0f,
        0.0f, 
        0.0f
    }; 
    return ret; 
}

INLINE float Vector4DotProduct(VECTOR4_INPUT l, VECTOR4_INPUT r)
{
    return  l.x * r.x + 
            l.y * r.y + 
            l.z * r.z + 
            l.w * r.w;
}
INLINE Vector4 Vector4CrossProduct(VECTOR4_INPUT l, VECTOR4_INPUT r)
{ 
    Vector4 ret = 
    {
        l.y*r.z-l.z*r.y,
        l.z*r.x-l.x*r.z,
        l.x*r.y-l.y*r.x,
        0.0f
    };
    return ret; 
}
INLINE float Vector4Magnitude(VECTOR4_INPUT v)
{ 
    return sqrtf(Vector4DotProduct(v,v)); 
}
INLINE float Vector4MagnitudeSq(VECTOR4_INPUT v) 
{ 
    return Vector4DotProduct(v, v); 
}
INLINE Vector4 Vector4Normalize(VECTOR4_INPUT v) 
{
    return Vector4DivScalar(v, Vector4Magnitude(v));
}

INLINE Vector3 Vector3FromVector4(VECTOR4_INPUT v)
{
    Vector3 ret = 
    {
        v.x,
        v.y,
        v.z
    };
    return ret; 
}

/*
-------------------------------------------------------------------------------
Matrix 3
-------------------------------------------------------------------------------
*/
typedef struct Matrix3_t
{
    Vector3 r0; 
    Vector3 r1; 
    Vector3 r2; 
} Matrix3;

INLINE Matrix3 Matrix3FromArray(const float* f) 
{ 
    Matrix3 ret =
    { 
        {f[0],f[1],f[2]},                      
        {f[3],f[4],f[5]},
        {f[6],f[7],f[8]} 
    };
    return ret;
}

INLINE Matrix3 Matrix3Identity(void)
{
    Matrix3 ret =
    {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
    };
    return ret;
}

INLINE Matrix3 Matrix3FromVectors(VECTOR3_INPUT r0,VECTOR3_INPUT r1,VECTOR3_INPUT r2)
{
    Matrix3 ret;
    ret.r0 = r0;
    ret.r1 = r1;
    ret.r2 = r2;
    return ret;
}
INLINE Matrix3 Matrix3Add(MATRIX3_INPUT l, MATRIX3_INPUT r)
{
    Matrix3 ret;
    ret.r0 = Vector3Add(l.r0, r.r0);
    ret.r1 = Vector3Add(l.r1, r.r1);
    ret.r2 = Vector3Add(l.r2, r.r2);
    return ret;
}
INLINE Matrix3 Matrix3Sub(MATRIX3_INPUT l, MATRIX3_INPUT r)
{
    Matrix3 ret;
    ret.r0 = Vector3Sub(l.r0, r.r0);
    ret.r1 = Vector3Sub(l.r1, r.r1);
    ret.r2 = Vector3Sub(l.r2, r.r2);
    return ret;
}
INLINE Matrix3 Matrix3MulScalar(MATRIX3_INPUT m, float f)
{
    Matrix3 ret;
    ret.r0 = Vector3MulScalar(m.r0, f);
    ret.r1 = Vector3MulScalar(m.r1, f);
    ret.r2 = Vector3MulScalar(m.r2, f);
    return ret;
}
INLINE Matrix3 Matrix3DivScalar(MATRIX3_INPUT m, float f)
{
    Matrix3 ret;
    ret.r0 = Vector3DivScalar(m.r0,f);
    ret.r1 = Vector3DivScalar(m.r1,f);
    ret.r2 = Vector3DivScalar(m.r2,f);
    return ret;
    
}
INLINE int Matrix3Equal(MATRIX3_INPUT l, MATRIX3_INPUT r) 
{ 
    return  Vector3Equal(l.r0, r.r0) && 
            Vector3Equal(l.r1, r.r1) &&
            Vector3Equal(l.r2, r.r2);
}
#include <stddef.h>
INLINE Matrix3 Matrix3MatrixMultiply(MATRIX3_INPUT l, MATRIX3_INPUT r)
{
    Matrix3 m = Matrix3Identity();
    
    float* left     = (float*)&l;
    float* right    = (float*)&r;
    float* result   = (float*)&m;
    
    int ii, jj, kk;
    for(ii=0; ii<3; ++ii) /* row */
    {
        for(jj=0; jj<3; ++jj) /* column */
        {
            float sum = MTX3_INDEX(left,ii,0)*MTX3_INDEX(right,0,jj);
            for(kk=1; kk<3; ++kk) 
            {
                sum += (MTX3_INDEX(left,ii,kk)*MTX3_INDEX(right,kk,jj));
            }
            MTX3_INDEX(result,ii,jj) = sum;
        }
    }
    return m;
}
INLINE Vector3 Matrix3VectorMultiply(MATRIX3_INPUT m,VECTOR3_INPUT v)
{
    Vector3 a;
    
    a.x = Vector3Hadd(Vector3Multiply(m.r0,v));
    a.y = Vector3Hadd(Vector3Multiply(m.r1,v));
    a.z = Vector3Hadd(Vector3Multiply(m.r2,v));
    
    return a;
}
INLINE float Matrix3Determinant(MATRIX3_INPUT m)
{
    float f0 = m.r0.x *  (m.r1.y*m.r2.z-m.r2.y*m.r1.z);
    float f1 = m.r0.y * -(m.r1.x*m.r2.z-m.r2.x*m.r1.z);
    float f2 = m.r0.z *  (m.r1.x*m.r2.y-m.r2.x*m.r1.y);
    
    return f0 + f1 + f2;
}
INLINE Matrix3 Matrix3Transpose(MATRIX3_INPUT m)
{   
    Matrix3 trans = m;
    
    SwapFloat(&trans.r0.y, &trans.r1.x);
    SwapFloat(&trans.r0.z, &trans.r2.x);    
    SwapFloat(&trans.r1.z, &trans.r2.y);
    
    return trans;
}

INLINE Matrix3 Matrix3Inverse(MATRIX3_INPUT m)
{    
    float det = Matrix3Determinant(m);
    Matrix3 inv;
    
    inv.r0.x =   (m.r1.y*m.r2.z) - (m.r1.z*m.r2.y);
    inv.r0.y = -((m.r1.x*m.r2.z) - (m.r1.z*m.r2.x));
    inv.r0.z =   (m.r1.x*m.r2.y) - (m.r1.y*m.r2.x);
    
    inv.r1.x = -((m.r0.y*m.r2.z) - (m.r0.z*m.r2.y));
    inv.r1.y =   (m.r0.x*m.r2.z) - (m.r0.z*m.r2.x);
    inv.r1.z = -((m.r0.x*m.r2.y) - (m.r0.y*m.r2.x));
    
    inv.r2.x =   (m.r0.y*m.r1.z) - (m.r0.z*m.r1.y);
    inv.r2.y = -((m.r0.x*m.r1.z) - (m.r0.z*m.r1.x));
    inv.r2.z =   (m.r0.x*m.r1.y) - (m.r0.y*m.r1.x);
    
    inv = Matrix3Transpose(inv);
    inv = Matrix3DivScalar(inv, det);
    
    return inv;
}

INLINE Matrix3 Matrix3RotationX(float rad)
{
    float       c = cosf( rad );
    float       s = sinf( rad );
    Matrix3     m = 
    {  
        {1.0f, 0.0f, 0.0f},
        {0.0f,    c,    s},
        {0.0f,   -s,    c} 
    };
    return m;
}
INLINE Matrix3 Matrix3RotationY(float rad)
{    
    float       c = cosf( rad );
    float       s = sinf( rad );
    Matrix3     m = 
    {
        {   c, 0.0f,   -s},
        {0.0f, 1.0f, 0.0f},
        {   s, 0.0f,    c}
    };
    return m;
}
INLINE Matrix3 Matrix3RotationZ(float rad)
{
    
    float       c = cosf( rad );
    float       s = sinf( rad );
    Matrix3     m =
    { 
        {   c,    s, 0.0f},
        {  -s,    c, 0.0f},
        {0.0f, 0.0f, 1.0f} 
    };
    return m;
}
INLINE Matrix3 Matrix3RotationAxis(VECTOR3_INPUT axis, float rad )
{
    float c = cosf(rad);
    float s = sinf(rad);
    float t = 1 - c;
    
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    
    Matrix3 m;
    
    m.r0.x = (t * x * x) + c;
    m.r0.y = (t * x * y) + s * z;
    m.r0.z = (t * x * z) - s * y;
    
    m.r1.x = (t * x * y) - (s * z);
    m.r1.y = (t * y * y) + c;
    m.r1.z = (t * y * z) + (s * x);
    
    m.r2.x = (t * x * z) + (s * y);
    m.r2.y = (t * y * z) - (s * x);
    m.r2.z = (t * z * z) + c;
    
    return m;
}
INLINE Matrix3 Matrix3Scale(float scl)
{
    Matrix3 m = 
    {   
        { scl, 0.0f, 0.0f},
        {0.0f,  scl, 0.0f},
        {0.0f, 0.0f,  scl} 
    };
    return m;
}

/*
-------------------------------------------------------------------------------
Matrix 3
-------------------------------------------------------------------------------
*/
typedef struct Matrix4_t
{
    Vector4 r0;
    Vector4 r1;
    Vector4 r2;
    Vector4 r3;
} Matrix4;

INLINE Matrix4 Matrix4FromArray(const float* f)
{
    Matrix4 ret = 
    { 
        {f[0],  f[1], f[2], f[3]},
        {f[4],  f[5], f[6], f[7]},
        {f[8],  f[9],f[10],f[11]},
        {f[12],f[13],f[14],f[15]}
    };
    return ret;
}
INLINE Matrix4 Matrix4Identity(void)
{
    Matrix4 ret = 
    {   
        {1.0f,0.0f,0.0f,0.0f},
        {0.0f,1.0f,0.0f,0.0f},
        {0.0f,0.0f,1.0f,0.0f},
        {0.0f,0.0f,0.0f,1.0f}
    };
    return ret;
}
INLINE Matrix4 Matrix4FromMatrix3(MATRIX3_INPUT m)
{
    Matrix4 ret = 
    {   
        { m.r0.x, m.r0.y, m.r0.z, 0.0f},
        { m.r1.x, m.r1.y, m.r1.z, 0.0f},
        { m.r2.x, m.r2.y, m.r2.z, 0.0f},
        {   0.0f,   0.0f,   0.0f, 1.0f}
    };
    return ret;
}
INLINE Matrix4 Matrix4FromVectors(VECTOR4_INPUT r0, VECTOR4_INPUT r1, VECTOR4_INPUT r2, VECTOR4_INPUT r3) 
{ 
    Matrix4 ret;
    ret.r0 = r0;
    ret.r1 = r1;
    ret.r2 = r2;
    ret.r3 = r3;
    return ret;
}

INLINE Matrix4 Matrix4Add(MATRIX4_INPUT l, MATRIX4_INPUT r)
{
    Matrix4 ret;
    ret.r0 = Vector4Add(l.r0, r.r0);
    ret.r1 = Vector4Add(l.r1, r.r1);
    ret.r2 = Vector4Add(l.r2, r.r2);
    ret.r3 = Vector4Add(l.r3, r.r3);
    return ret;
}
INLINE Matrix4 Matrix4Sub(MATRIX4_INPUT l, MATRIX4_INPUT r)
{
    Matrix4 ret;
    ret.r0 = Vector4Sub(l.r0, r.r0);
    ret.r1 = Vector4Sub(l.r1, r.r1);
    ret.r2 = Vector4Sub(l.r2, r.r2);
    ret.r3 = Vector4Sub(l.r3, r.r3);
    return ret;
}
INLINE Matrix4 Matrix4MulScalar(MATRIX4_INPUT m, float f)
{
    Matrix4 ret;
    ret.r0 = Vector4MulScalar(m.r0, f);
    ret.r1 = Vector4MulScalar(m.r1, f);
    ret.r2 = Vector4MulScalar(m.r2, f);
    ret.r3 = Vector4MulScalar(m.r3, f);
    return ret;
    
}
INLINE Matrix4 Matrix4DivScalar(MATRIX4_INPUT m, float f)
{
    float recip = 1.0f/f;
    Matrix4 ret;
    ret.r0 = Vector4MulScalar(m.r0, recip);
    ret.r1 = Vector4MulScalar(m.r1, recip);
    ret.r2 = Vector4MulScalar(m.r2, recip);
    ret.r3 = Vector4MulScalar(m.r3, recip);
    return ret;    
}
INLINE int Matrix4Equal(MATRIX4_INPUT l, MATRIX4_INPUT r) 
{ 
    return  Vector4Equal(l.r0, r.r0) && 
            Vector4Equal(l.r1, r.r1) &&
            Vector4Equal(l.r2, r.r2) &&
            Vector4Equal(l.r3, r.r3);
}

INLINE Matrix4 Matrix4Transpose(MATRIX4_INPUT mat)
{
    Matrix4 m = mat;
    
    SwapFloat(&m.r0.y,&m.r1.x);
    SwapFloat(&m.r0.z,&m.r2.x);
    SwapFloat(&m.r0.w,&m.r3.x);
    
    SwapFloat(&m.r1.z,&m.r2.y);
    SwapFloat(&m.r1.w,&m.r3.y);
    
    SwapFloat(&m.r2.w,&m.r3.z);
    
    return m;
}
INLINE Vector4 Matrix4VectorMultiply(MATRIX4_INPUT m, VECTOR4_INPUT v)
{
#if 1
    Matrix4 t = Matrix4Transpose(m);
#else
    Matrix4 t = m;
#endif
    
    Vector4 x = Vector4MulScalar(t.r0, v.x);
    Vector4 y = Vector4MulScalar(t.r1, v.y);
    Vector4 z = Vector4MulScalar(t.r2, v.z);
    Vector4 w = Vector4MulScalar(t.r3, v.w);
    
    Vector4 t1 = Vector4Add(x,y);
    Vector4 t2 = Vector4Add(z,w);
    
    Vector4 r = Vector4Add(t1, t2);
    
    return r;
}
INLINE Matrix4 Matrix4MatrixMultiply(MATRIX4_INPUT l, MATRIX4_INPUT r)
{
    Matrix4 m;
    
    float* left     = (float*)&l;
    float* right    = (float*)&r;
    float* result   = (float*)&m;
    
    int ii, jj, kk;
    for(ii=0; ii < 4; ++ii) /* rows */
    {
        for(jj=0; jj<4; ++jj) /* colomns */
        {
            float sum = MTX4_INDEX(left,ii,0)*MTX4_INDEX(right,0,jj);
            for(kk=1; kk<4; ++kk) 
            {
                sum += MTX4_INDEX(left,ii,kk)*MTX4_INDEX(right,kk,jj);
            }
            MTX4_INDEX(result,ii,jj) = sum;
        }
    }
    
    return m;
}

INLINE float   Matrix4Determinant(MATRIX4_INPUT m)
{
    float det = 0.0f;
    
    Matrix3 a = 
    { 
        {m.r1.y,m.r1.z,m.r1.w}, 
        {m.r2.y,m.r2.z,m.r2.w}, 
        {m.r3.y,m.r3.z,m.r3.w}
    };
    
    Matrix3 b = 
    { 
        {m.r1.x,m.r1.z,m.r1.w}, 
        {m.r2.x,m.r2.z,m.r2.w}, 
        {m.r3.x,m.r3.z,m.r3.w}
    };
    
    Matrix3 c = 
    { 
        {m.r1.x,m.r1.y,m.r1.w}, 
        {m.r2.x,m.r2.y,m.r2.w}, 
        {m.r3.x,m.r3.y,m.r3.w}
    };
    
    Matrix3 d = { 
        {m.r1.x,m.r1.y,m.r1.z}, 
        {m.r2.x,m.r2.y,m.r2.z}, 
        {m.r3.x,m.r3.y,m.r3.z}
    };
                 
    
    det += m.r0.x * Matrix3Determinant(a);
    
    det -= m.r0.y * Matrix3Determinant(b);
    
    det += m.r0.z * Matrix3Determinant(c);
    
    det -= m.r0.w * Matrix3Determinant(d);
    
    return det;
}
INLINE Matrix4 Matrix4Inverse(MATRIX4_INPUT in)
{
    Matrix4 ret;
    float recip;    
    
    /* temp matrices */
    
    /* row 1 */
    Matrix3 a = {{in.r1.y,in.r1.z,in.r1.w}, 
                {in.r2.y,in.r2.z,in.r2.w}, 
                {in.r3.y,in.r3.z,in.r3.w}};
    
    Matrix3 b = {{in.r1.x,in.r1.z,in.r1.w}, 
                {in.r2.x,in.r2.z,in.r2.w}, 
                {in.r3.x,in.r3.z,in.r3.w}};
    
    Matrix3 c = {{in.r1.x,in.r1.y,in.r1.w}, 
                {in.r2.x,in.r2.y,in.r2.w}, 
                {in.r3.x,in.r3.y,in.r3.w}};
    
    Matrix3 d = {{in.r1.x,in.r1.y,in.r1.z}, 
                {in.r2.x,in.r2.y,in.r2.z}, 
                {in.r3.x,in.r3.y,in.r3.z}};
              
    /* row 2 */
    Matrix3 e = {{in.r0.y,in.r0.z,in.r0.w}, 
                {in.r2.y,in.r2.z,in.r2.w}, 
                {in.r3.y,in.r3.z,in.r3.w}};
    
    Matrix3 f = {{in.r0.x,in.r0.z,in.r0.w}, 
                {in.r2.x,in.r2.z,in.r2.w}, 
                {in.r3.x,in.r3.z,in.r3.w}};
    
    Matrix3 g = {{in.r0.x,in.r0.y,in.r0.w}, 
                {in.r2.x,in.r2.y,in.r2.w}, 
                {in.r3.x,in.r3.y,in.r3.w}};
    
    Matrix3 h = {{in.r0.x,in.r0.y,in.r0.z}, 
                {in.r2.x,in.r2.y,in.r2.z}, 
                {in.r3.x,in.r3.y,in.r3.z}};
                
            
    /* row 3 */    
    Matrix3 i = {{in.r0.y,in.r0.z,in.r0.w}, 
                {in.r1.y,in.r1.z,in.r1.w}, 
                {in.r3.y,in.r3.z,in.r3.w}};
    
    Matrix3 j = {{in.r0.x,in.r0.z,in.r0.w}, 
                {in.r1.x,in.r1.z,in.r1.w}, 
                {in.r3.x,in.r3.z,in.r3.w}};
    
    Matrix3 k = {{in.r0.x,in.r0.y,in.r0.w}, 
                {in.r1.x,in.r1.y,in.r1.w}, 
                {in.r3.x,in.r3.y,in.r3.w}};
    
    Matrix3 l = {{in.r0.x,in.r0.y,in.r0.z}, 
                {in.r1.x,in.r1.y,in.r1.z}, 
                {in.r3.x,in.r3.y,in.r3.z}};
                
         
    /* row 4 */       
    Matrix3 m = {{in.r0.y,in.r0.z,in.r0.w}, 
                {in.r1.y,in.r1.z,in.r1.w}, 
                {in.r2.y,in.r2.z,in.r2.w}};
    
    Matrix3 n = {{in.r0.x,in.r0.z,in.r0.w}, 
                {in.r1.x,in.r1.z,in.r1.w}, 
                {in.r2.x,in.r2.z,in.r2.w}};
    
    Matrix3 o = {{in.r0.x,in.r0.y,in.r0.w}, 
                {in.r1.x,in.r1.y,in.r1.w}, 
                {in.r2.x,in.r2.y,in.r2.w}};
    
    Matrix3 p = {{in.r0.x,in.r0.y,in.r0.z}, 
                {in.r1.x,in.r1.y,in.r1.z}, 
                {in.r2.x,in.r2.y,in.r2.z}};
    
    /* row 1 */
    ret.r0.x = Matrix3Determinant(a);
    
    ret.r0.y = -Matrix3Determinant(b);
    
    ret.r0.z = Matrix3Determinant(c);
    
    ret.r0.w = -Matrix3Determinant(d);
    
    /* row 2 */
    ret.r1.x = -Matrix3Determinant(e);
    
    ret.r1.y = Matrix3Determinant(f);
    
    ret.r1.z = -Matrix3Determinant(g);
    
    ret.r1.w = Matrix3Determinant(h);
    
    /* row 3 */
    ret.r2.x = Matrix3Determinant(i);
    
    ret.r2.y = -Matrix3Determinant(j);
    
    ret.r2.z = Matrix3Determinant(k);
    
    ret.r2.w = -Matrix3Determinant(l);
    
    /* row 4 */
    ret.r3.x = -Matrix3Determinant(m);
    
    ret.r3.y = Matrix3Determinant(n);
    
    ret.r3.z = -Matrix3Determinant(o);
    
    ret.r3.w = Matrix3Determinant(p);
    
    ret = Matrix4Transpose( ret );
    recip = 1.0f/Matrix4Determinant( in );
    ret = Matrix4MulScalar(ret, recip);
    
    return ret;
}

INLINE Matrix4 Matrix4RotationX(float rad)
{    
    float c = cosf( rad );
    float s = sinf( rad );
    Matrix4 m = 
    {  
        {1.0f, 0.0f, 0.0f, 0.0f}, 
        {0.0f,    c,    s, 0.0f}, 
        {0.0f,   -s,    c, 0.0f}, 
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    return m;
}
INLINE Matrix4 Matrix4RotationY(float rad)
{
    float c = cosf( rad );
    float s = sinf( rad );
    Matrix4 m =  
    { 
        {   c, 0.0f,   -s, 0.0f}, 
        {0.0f, 1.0f, 0.0f, 0.0f}, 
        {   s, 0.0f,    c, 0.0f}, 
        {0.0f, 0.0f, 0.0f, 1.0f} 
    };
    
    return m;
}
INLINE Matrix4 Matrix4RotationZ(float rad)
{
    float c = cosf( rad );
    float s = sinf( rad );
    Matrix4    m = 
    {   
        {   c,    s, 0.0f, 0.0f}, 
        {  -s,    c, 0.0f, 0.0f}, 
        {0.0f, 0.0f, 1.0f, 0.0f}, 
        {0.0f, 0.0f, 0.0f, 1.0f} 
    };
    return m;
}
INLINE Matrix4 Matrix4RotationAxis(VECTOR3_INPUT axis, float rad )
{
    float c = cosf(rad);
    float s = sinf(rad);
    float t = 1 - c;
    
    float x = axis.x;
    float y = axis.y;
    float z = axis.z;
    
    Matrix4 m = Matrix4Identity();
    
    m.r0.x = (t * x * x) + c;
    m.r0.y = (t * x * y) - s * z;
    m.r0.z = (t * x * z) + s * y;
    
    m.r1.x = (t * x * y) + (s * z);
    m.r1.y = (t * y * y) + c;
    m.r1.z = (t * y * z) - (s * x);
    
    m.r2.x = (t * x * z) - (s * y);
    m.r2.y = (t * y * z) + (s * x);
    m.r2.z = (t * z * z) + c;
    
    return m;
}
INLINE Matrix4 Matrix4UniformScale(float scl)
{
    Matrix4 m = 
    {   
        { scl, 0.0f, 0.0f, 0.0f}, 
        {0.0f,  scl, 0.0f, 0.0f}, 
        {0.0f, 0.0f,  scl, 0.0f}, 
        {0.0f, 0.0f, 0.0f, 1.0f}
    };
    return m;
}
INLINE Matrix4 Matrix4Scale(float x, float y, float z)
{
    Matrix4 m = 
    {  
        {   x, 0.0f, 0.0f, 0.0f}, 
        {0.0f,    y, 0.0f, 0.0f}, 
        {0.0f, 0.0f,    z, 0.0f}, 
        {0.0f, 0.0f, 0.0f, 1.0f} 
    };
    return m;
}
INLINE Matrix4 Matrix4TranslationFloat(float x, float y, float z )
{
    Matrix4 m = 
    {  
        {1.0f, 0.0f, 0.0f, 0.0f}, 
        {0.0f, 1.0f, 0.0f, 0.0f}, 
        {0.0f, 0.0f, 1.0f, 0.0f}, 
        {   x,    y,    z, 1.0f} 
    };
    
    return m;
}
INLINE Matrix4 Matrix4TranslationVector(VECTOR3_INPUT t)
{
    return Matrix4TranslationFloat(t.x, t.y, t.z);
}
INLINE Matrix4 Matrix4PerspectiveFovLH(float fov, float aspect, float nearPlane, float farPlane)
{    
    Matrix4 m = Matrix4Identity();

    float f = tanf(kPiDiv2 - (fov/2));
    float diff = farPlane-nearPlane;
    float div = farPlane / diff;

    m.r0.x = f/aspect;
    m.r1.y = f;
    m.r2.z = div;
    m.r2.w = 1;
    m.r3.z = -nearPlane*div;
    m.r3.w = 0;

    return m;
}
INLINE Matrix4 Matrix4PerspectiveFovRH(float fov, float aspect, float nearPlane, float farPlane)
{    
    Matrix4 m = Matrix4Identity();
    
    float f = tanf(kPiDiv2 - (fov/2));
    float diff = nearPlane-farPlane;
    float div = farPlane / diff;

    m.r0.x = f / aspect;
    m.r1.y = f;
    m.r2.z = div;
    m.r2.w = -1;
    m.r3.z = nearPlane * div;
    m.r3.w = 0;
    return m;
}
INLINE Matrix4 Matrix4PerspectiveLH(float width, float height, float nearPlane, float farPlane)
{    
    Matrix4 m = Matrix4Identity();
    
    m.r0.x = 2*nearPlane/width;
    m.r1.y = 2*nearPlane/height;
    m.r2.z = farPlane/(farPlane-nearPlane);
    m.r2.w = 1;
    m.r3.z = nearPlane*farPlane/(nearPlane-farPlane);
    m.r3.w = 0;
    return m;
}
INLINE Matrix4 Matrix4PerspectiveRH(float width, float height, float nearPlane, float farPlane)
{    
    Matrix4 m = Matrix4Identity();
    
    m.r0.x = 2*nearPlane/width;
    m.r1.y = 2*nearPlane/height;
    m.r2.z = farPlane/(nearPlane-farPlane);
    m.r2.w = -1;
    m.r3.z = nearPlane*farPlane/(nearPlane-farPlane);
    m.r3.w = 0;
    return m;
}
INLINE Matrix4 Matrix4OrthographicOffCenterLH(float left, float right, float top, float bottom, float nearPlane, float farPlane)
{
    Matrix4 m = Matrix4Identity();
    
    float diff = farPlane-nearPlane;
    
    m.r0.x = 2.0f/(right-left);
    m.r1.y = 2.0f/(top-bottom);
    m.r2.z = 1.0f/diff;
    m.r3.x = -((left+right)/(right-left));
    m.r3.y = -((top+bottom)/(top-bottom));
    m.r3.z = -nearPlane/diff;

    return m;
}
INLINE Matrix4 Matrix4OrthographicOffCenterRH(float left, float right, float top, float bottom, float nearPlane, float farPlane)
{
    Matrix4 m = Matrix4Identity();
    float diff = nearPlane-farPlane;
    
    m.r0.x = 2.0f/(right-left);
    m.r1.y = 2.0f/(top-bottom);
    m.r2.z = 1.0f/diff;
    m.r3.x = -((left+right)/(right-left));
    m.r3.y = -((top+bottom)/(top-bottom));
    m.r3.z = nearPlane/diff;
    
    return m;
}
INLINE Matrix4 Matrix4OrthographicLH(float width, float height, float nearPlane, float farPlane)
{
    float halfWidth = width/2.0f;
    float halfHeight = height/2.0f;
    
    return Matrix4OrthographicOffCenterLH(-halfWidth, halfWidth, halfHeight, -halfHeight, nearPlane, farPlane);
}
INLINE Matrix4 Matrix4OrthographicRH(float width, float height, float nearPlane, float farPlane)
{
    float halfWidth = width/2.0f;
    float halfHeight = height/2.0f;
    
    return Matrix4OrthographicOffCenterRH(-halfWidth, halfWidth, halfHeight, -halfHeight, nearPlane, farPlane);
}
INLINE Vector3 Matrix4GetXAxis(MATRIX4_INPUT m)
{
    return Vector3FromArray(&m.r0.x);
}
INLINE Vector3 Matrix4GetYAxis(MATRIX4_INPUT m)
{
    return Vector3FromArray(&m.r1.x);
}
INLINE Vector3 Matrix4GetZAxis(MATRIX4_INPUT m)
{
    return Vector3FromArray(&m.r2.x);
}
INLINE Vector3 Matrix4GetTranslation(MATRIX4_INPUT m)
{
    return Vector3FromArray(&m.r3.x);
}

/*
//-----------------------------------------------------------------------------
//  Quaternion
//-----------------------------------------------------------------------------
*/
typedef Vector4 Quaternion;

INLINE Vector3 QuaternionGetXAxis(QUATERNION_INPUT q) 
{ 
    Vector3 ret = 
    { 
        1-2*(q.y*q.y+q.z*q.z),
          2*(q.x*q.y+q.w*q.z), 
          2*(q.x*q.z-q.y*q.w) 
    }; 
    return ret; 
}
INLINE Vector3 QuaternionGetYAxis(QUATERNION_INPUT q) 
{ 
    Vector3 ret = 
    { 
          2*(q.x*q.y-q.z*q.w), 
        1-2*(q.x*q.x+q.z*q.z), 
          2*(q.y*q.z+q.x*q.w) 
    }; 
    return ret; 
}
INLINE Vector3 QuaternionGetZAxis(QUATERNION_INPUT q) 
{ 
    Vector3 ret = 
    { 
          2*(q.x*q.z+q.y*q.w), 
          2*(q.y*q.z-q.x*q.w), 
        1-2*(q.x*q.x+q.y*q.y) 
    }; 
    return ret; 
}
INLINE Matrix3 QuaternionGetMatrix(QUATERNION_INPUT q) 
{ 
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    
    float xy = q.x * q.y;
    float zw = q.z * q.w;
    float xz = q.x * q.z;
    float yw = q.y * q.w;
    float yz = q.y * q.z;
    float xw = q.x * q.w;
    
    Matrix3 ret = 
    {
        { 1 - 2*(yy+zz),     2*(xy-zw),     2*(xz+yw) },
        {     2*(xy+zw), 1 - 2*(xx+zz),     2*(yz-xw) },
        {     2*(xz-yw),     2*(yz+xw), 1 - 2*(xx+yy) },
    };
    return ret;
}
INLINE Vector3 QuaternionRotateVector3(QUATERNION_INPUT q, VECTOR3_INPUT v)
{
    Matrix3 mat = QuaternionGetMatrix(q);
    Vector3 ret = Matrix3VectorMultiply(mat, v);
    return ret;
}
INLINE Vector4 QuaternionRotateVector4(QUATERNION_INPUT q,VECTOR4_INPUT v)
{
    Vector3 vec = Vector3FromVector4(v);
    Vector3 result = QuaternionRotateVector3(q,vec);
    return Vector4FromVector3(result, v.w);
}
INLINE Quaternion QuatFromAxisAngle(VECTOR3_INPUT axis, float angle)
{
    Quaternion q;
    Vector3 v = Vector3Normalize( axis );
    float   a = angle*0.5f;
    float   s = sinf(a);
    q.x = v.x*s;
    q.y = v.y*s;
    q.z = v.z*s;
    q.w = cosf(a);
    return q;
}
INLINE Quaternion QuatRotationX(float angle)
{
    float a = angle/2.0f;
    Vector3 v = {sinf( a ), 0.0f, 0.0f};
    Quaternion q = { v.x, v.y, v.z, cosf( a ) };
    return q;
}
INLINE Quaternion QuatRotationY( float angle)
{
    float a = angle/ 2.0f;
    Vector3 v = {0.0f, sinf( a ), 0.0f};
    Quaternion q = { v.x, v.y, v.z, cosf( a ) };
    return q;
}
INLINE Quaternion QuatRotationZ(float angle)
{
    float a = angle / 2.0f;
    Vector3 v = {0.0f, 0.0f, sinf( a ) };
    Quaternion q = { v.x, v.y, v.z, cosf( a ) };
    return q;
}
INLINE Quaternion QuaternionQuaternionMul(QUATERNION_INPUT l, QUATERNION_INPUT r)
{
    Quaternion q =
    {
        l.w*r.x + l.x*r.w + l.y*r.z - l.z*r.y,
        l.w*r.y - l.x*r.z + l.y*r.w + l.z*r.x,
        l.w*r.z + l.x*r.y - l.y*r.x + l.z*r.w,
        l.w*r.w - l.x*r.x - l.y*r.y - l.z*r.z 
    };
    return q;
}
INLINE Quaternion QuaternionConjugate(QUATERNION_INPUT q) 
{ 
    Quaternion ret = 
    {
        -q.x,
        -q.y,
        -q.z,
        q.w
    }; 
    return ret;
}
INLINE Quaternion QuaternionInverse(QUATERNION_INPUT q) 
{ 
    return QuaternionConjugate(q); 
}
INLINE Quaternion QuaternionZero(void) 
{ 
    Quaternion ret = {0.0f,0.0f,0.0f,1.0f}; 
    return ret; 
}
INLINE Quaternion QuaternionLerp(QUATERNION_INPUT a, QUATERNION_INPUT b, float t)
{    
    float inv = 1.0f - t;
    Quaternion q = 
    { 
        inv*a.x + t*b.x,
        inv*a.y + t*b.y,
        inv*a.z + t*b.z,
        inv*a.w + t*b.w 
    };
    return q;
}

/*
//-----------------------------------------------------------------------------
//  Transform
//-----------------------------------------------------------------------------
*/
typedef struct Transform_t
{
    Quaternion  orientation;
    Vector3     position;
    float       scale;
} Transform;

INLINE Transform TransformZero(void)
{
    Transform t;
    t.orientation = QuaternionZero();
    t.position  = Vector3Zero();
    t.scale = 1.0f;
    return t;
}

INLINE Matrix4 TransformGetMatrix(TRANSFORM_INPUT t)
{
    Quaternion q = t.orientation;/* Vector4Normalize(t.orientation); */
    float xx = q.x * q.x;
    float yy = q.y * q.y;
    float zz = q.z * q.z;
    
    float xy = q.x * q.y;
    float zw = q.z * q.w;
    float xz = q.x * q.z;
    float yw = q.y * q.w;
    float yz = q.y * q.z;
    float xw = q.x * q.w;
    
    #if 0
    Matrix4 ret = 
    {
        { 1 - 2*(yy+zz),     2*(xy-zw),     2*(xz+yw), 0.0f },
        {     2*(xy+zw), 1 - 2*(xx+zz),     2*(yz-xw), 0.0f },
        {     2*(xz-yw),     2*(yz+xw), 1 - 2*(xx+yy), 0.0f },
        {  t.position.x,  t.position.y,  t.position.z, 1.0f },
    };
    #else
    Matrix4 ret = 
    {
        { 1 - 2*(yy+zz),     2*(xy+zw),     2*(xz-yw), 0.0f },
        {     2*(xy-zw), 1 - 2*(xx+zz),     2*(yz+xw), 0.0f },
        {     2*(xz+yw),     2*(yz-xw), 1 - 2*(xx+yy), 0.0f },
        {  t.position.x,  t.position.y,  t.position.z, 1.0f },
    };
    if(t.scale != 1.0f)
    {
        ret.r0.x *= t.scale;
        ret.r0.y *= t.scale;
        ret.r0.z *= t.scale;
        ret.r1.x *= t.scale;
        ret.r1.y *= t.scale;
        ret.r1.z *= t.scale;
        ret.r2.x *= t.scale;
        ret.r2.y *= t.scale;
        ret.r2.z *= t.scale;
    }
    #endif
    return ret;
}

INLINE void TransformTranslateX(Transform* transform, float t)
{
    Vector3 x = QuaternionGetXAxis(transform->orientation);
    x = Vector3MulScalar(x, t);
    transform->position = Vector3Add(transform->position, x);
}
INLINE void TransformTranslateY(Transform* transform, float t)
{
    Vector3 y = {0.0f, 1.0f, 0.0f};
    y = Vector3MulScalar(y, t);
    transform->position = Vector3Add(transform->position, y);
}
INLINE void TransformTranslateZ(Transform* transform, float t)
{
    Vector3 z = QuaternionGetZAxis(transform->orientation);
    z = Vector3MulScalar(z, t);
    transform->position = Vector3Add(transform->position, z);
}

INLINE void TransformRotateX(Transform* transform, float r)
{
    Vector3 xAxis = {1.0f, 0.0f, 0.0f};
    Quaternion q = QuatFromAxisAngle(xAxis, r);
    transform->orientation = QuaternionQuaternionMul(transform->orientation, q);
}
INLINE void TransformRotateY(Transform* transform, float r)
{
    Vector3 yAxis = {0.0f, 1.0f, 0.0f};
    Quaternion q = QuatFromAxisAngle(yAxis, r);
    transform->orientation = QuaternionQuaternionMul(q, transform->orientation);
}

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} // extern "C" {
#endif

#endif
