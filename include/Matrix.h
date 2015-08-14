#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "Common.h"

#include <math.h>
#include <string.h>

#define PI 3.1415926535897932384626433832795f

typedef struct Matrix
{
    float m[4][4];
} Matrix;

float* Matrix_getOneDimensionalArray(Matrix*);
void Matrix_scale(Matrix* result, float sx, float sy, float sz);
void Matrix_translate(Matrix* result, float tx, float ty, float tz);
void Matrix_rotate(Matrix* result, float angle, float x, float y, float z);
void Matrix_loadIdentity(Matrix* result);
void Matrix_lookAt(Matrix* result, float posX, float posY, float posZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ);
void Matrix_frustum(Matrix* result, float left, float right, float bottom, float top, float nearZ, float farZ);
void Matrix_perspective(Matrix* result, float fovy, float aspect, float nearZ, float farZ);
void Matrix_ortho(Matrix* result, float left, float right, float bottom, float top, float nearZ, float farZ);
void Matrix_multiply(Matrix* result, Matrix* srcA, Matrix* srcB);

#endif /* _MATRIX_ */
