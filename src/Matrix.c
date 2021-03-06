#include "Matrix.h"

float* Matrix_getOneDimensionalArray(Matrix* matrix)
{
    float *a = NULL;
    a = malloc(16 * sizeof(float));
    assert(a);
    assert(matrix);
    a[0] = matrix->m[0][0];
    a[1] = matrix->m[0][1];
    a[2] = matrix->m[0][2];
    a[3] = matrix->m[0][3];
    a[4] = matrix->m[1][0];
    a[5] = matrix->m[1][1];
    a[6] = matrix->m[1][2];
    a[7] = matrix->m[1][3];
    a[8] = matrix->m[2][0];
    a[9] = matrix->m[2][1];
    a[10] = matrix->m[2][2];
    a[11] = matrix->m[2][3];
    a[12] = matrix->m[3][0];
    a[13] = matrix->m[3][1];
    a[14] = matrix->m[3][2];
    a[15] = matrix->m[3][3];
    return a;
}

/* Code lifted from MESA implementation of gluInvertMatrix(const double m[16], double invOut[16]) */
bool Matrix_inverse(Matrix* result, Matrix* source)
{
    double inv[16], det;
    int i;
    float* m;
    float* invOut;

    assert(source);

    m = (float*)source->m;
    invOut = (float*)result->m;

    inv[0] = m[5]  * m[10] * m[15] -
            m[5]  * m[11] * m[14] -
            m[9]  * m[6]  * m[15] +
            m[9]  * m[7]  * m[14] +
            m[13] * m[6]  * m[11] -
            m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
            m[4]  * m[11] * m[14] +
            m[8]  * m[6]  * m[15] -
            m[8]  * m[7]  * m[14] -
            m[12] * m[6]  * m[11] +
            m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
            m[4]  * m[11] * m[13] -
            m[8]  * m[5] * m[15] +
            m[8]  * m[7] * m[13] +
            m[12] * m[5] * m[11] -
            m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
            m[4]  * m[10] * m[13] +
            m[8]  * m[5] * m[14] -
            m[8]  * m[6] * m[13] -
            m[12] * m[5] * m[10] +
            m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
            m[1]  * m[11] * m[14] +
            m[9]  * m[2] * m[15] -
            m[9]  * m[3] * m[14] -
            m[13] * m[2] * m[11] +
            m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
            m[0]  * m[11] * m[14] -
            m[8]  * m[2] * m[15] +
            m[8]  * m[3] * m[14] +
            m[12] * m[2] * m[11] -
            m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
            m[0]  * m[11] * m[13] +
            m[8]  * m[1] * m[15] -
            m[8]  * m[3] * m[13] -
            m[12] * m[1] * m[11] +
            m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
            m[0]  * m[10] * m[13] -
            m[8]  * m[1] * m[14] +
            m[8]  * m[2] * m[13] +
            m[12] * m[1] * m[10] -
            m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
            m[1]  * m[7] * m[14] -
            m[5]  * m[2] * m[15] +
            m[5]  * m[3] * m[14] +
            m[13] * m[2] * m[7] -
            m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
            m[0]  * m[7] * m[14] +
            m[4]  * m[2] * m[15] -
            m[4]  * m[3] * m[14] -
            m[12] * m[2] * m[7] +
            m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
            m[0]  * m[7] * m[13] -
            m[4]  * m[1] * m[15] +
            m[4]  * m[3] * m[13] +
            m[12] * m[1] * m[7] -
            m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
            m[0]  * m[6] * m[13] +
            m[4]  * m[1] * m[14] -
            m[4]  * m[2] * m[13] -
            m[12] * m[1] * m[6] +
            m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
            m[1] * m[7] * m[10] +
            m[5] * m[2] * m[11] -
            m[5] * m[3] * m[10] -
            m[9] * m[2] * m[7] +
            m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
            m[0] * m[7] * m[10] -
            m[4] * m[2] * m[11] +
            m[4] * m[3] * m[10] +
            m[8] * m[2] * m[7] -
            m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
            m[0] * m[7] * m[9] +
            m[4] * m[1] * m[11] -
            m[4] * m[3] * m[9] -
            m[8] * m[1] * m[7] +
            m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
            m[0] * m[6] * m[9] -
            m[4] * m[1] * m[10] +
            m[4] * m[2] * m[9] +
            m[8] * m[1] * m[6] -
            m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0) return false;

    det = 1.f / det;

    for (i = 0; i < 16; i++)
    {
        invOut[i] = inv[i] * det;
    }
    return true;
}

void Matrix_scale(Matrix* result, float sx, float sy, float sz)
{
    result->m[0][0] *= sx;
    result->m[0][1] *= sx;
    result->m[0][2] *= sx;
    result->m[0][3] *= sx;

    result->m[1][0] *= sy;
    result->m[1][1] *= sy;
    result->m[1][2] *= sy;
    result->m[1][3] *= sy;

    result->m[2][0] *= sz;
    result->m[2][1] *= sz;
    result->m[2][2] *= sz;
    result->m[2][3] *= sz;
}

void Matrix_translate(Matrix* result, float tx, float ty, float tz)
{
    result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void Matrix_rotate(Matrix* result, float angle, float x, float y, float z)
{
    float sinAngle, cosAngle;
    float mag = sqrtf(x * x + y * y + z * z);

    sinAngle = sinf(angle * PI / 180.0f);
    cosAngle = cosf(angle * PI / 180.0f);

    if (mag > 0.0f)
    {
        float xx, yy, zz, xy, yz, zx, xs, ys, zs;
        float oneMinusCos;
        Matrix rotMat;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
        rotMat.m[0][1] = (oneMinusCos * xy) - zs;
        rotMat.m[0][2] = (oneMinusCos * zx) + ys;
        rotMat.m[0][3] = 0.0F;

        rotMat.m[1][0] = (oneMinusCos * xy) + zs;
        rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
        rotMat.m[1][2] = (oneMinusCos * yz) - xs;
        rotMat.m[1][3] = 0.0F;

        rotMat.m[2][0] = (oneMinusCos * zx) - ys;
        rotMat.m[2][1] = (oneMinusCos * yz) + xs;
        rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
        rotMat.m[2][3] = 0.0F;

        rotMat.m[3][0] = 0.0F;
        rotMat.m[3][1] = 0.0F;
        rotMat.m[3][2] = 0.0F;
        rotMat.m[3][3] = 1.0F;

        Matrix_multiply(result, &rotMat, result);
    }
}

void Matrix_frustum(Matrix* result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    Matrix frust;

    if ((nearZ <= 0.0f) || (farZ <= 0.0f) || (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f))
    {
        return;
    }

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    Matrix_multiply(result, &frust, result);
}

void Matrix_perspective(Matrix* result, float fovy, float aspect, float nearZ, float farZ)
{
    float frustumW, frustumH;

    frustumH = tanf(fovy / 360.0f * PI) * nearZ;
    frustumW = frustumH * aspect;

    Matrix_frustum(result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

void Matrix_ortho(Matrix* result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    Matrix ortho;

    if ((deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f))
    {
        return;
    }

    Matrix_loadIdentity(&ortho);
    ortho.m[0][0] = 2.0f / deltaX;
    ortho.m[3][0] = -(right + left) / deltaX;
    ortho.m[1][1] = 2.0f / deltaY;
    ortho.m[3][1] = -(top + bottom) / deltaY;
    ortho.m[2][2] = -2.0f / deltaZ;
    ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

    Matrix_multiply(result, &ortho, result);
}

void Matrix_multiply(Matrix* result, Matrix* srcA, Matrix* srcB)
{
    Matrix tmp;
    int i;

    Matrix_loadIdentity(result);

    for (i = 0; i < 4; i++)
    {
        tmp.m[i][0] = (srcA->m[i][0] * srcB->m[0][0]) + (srcA->m[i][1] * srcB->m[1][0]) + (srcA->m[i][2] * srcB->m[2][0])
                + (srcA->m[i][3] * srcB->m[3][0]);

        tmp.m[i][1] = (srcA->m[i][0] * srcB->m[0][1]) + (srcA->m[i][1] * srcB->m[1][1]) + (srcA->m[i][2] * srcB->m[2][1])
                + (srcA->m[i][3] * srcB->m[3][1]);

        tmp.m[i][2] = (srcA->m[i][0] * srcB->m[0][2]) + (srcA->m[i][1] * srcB->m[1][2]) + (srcA->m[i][2] * srcB->m[2][2])
                + (srcA->m[i][3] * srcB->m[3][2]);

        tmp.m[i][3] = (srcA->m[i][0] * srcB->m[0][3]) + (srcA->m[i][1] * srcB->m[1][3]) + (srcA->m[i][2] * srcB->m[2][3])
                + (srcA->m[i][3] * srcB->m[3][3]);
    }

    memcpy(result, &tmp, sizeof(Matrix));
}

void Matrix_loadIdentity(Matrix* result)
{
    memset(result, 0x0, sizeof(Matrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}

void Matrix_lookAt(Matrix* result, float posX, float posY, float posZ, float lookAtX, float lookAtY, float lookAtZ, float upX, float upY, float upZ)
{
    float axisX[3], axisY[3], axisZ[3];
    float length;
    axisZ[0] = lookAtX - posX;
    axisZ[1] = lookAtY - posY;
    axisZ[2] = lookAtZ - posZ;
    length = sqrtf(axisZ[0] * axisZ[0] + axisZ[1] * axisZ[1] + axisZ[2] * axisZ[2]);

    if (length != 0.0f)
    {
        axisZ[0] /= length;
        axisZ[1] /= length;
        axisZ[2] /= length;
    }
    axisX[0] = upY * axisZ[2] - upZ * axisZ[1];
    axisX[1] = upZ * axisZ[0] - upX * axisZ[2];
    axisX[2] = upX * axisZ[1] - upY * axisZ[0];
    length = sqrtf(axisX[0] * axisX[0] + axisX[1] * axisX[1] + axisX[2] * axisX[2]);

    if (length != 0.0f)
    {
        axisX[0] /= length;
        axisX[1] /= length;
        axisX[2] /= length;
    }

    axisY[0] = axisZ[1] * axisX[2] - axisZ[2] * axisX[1];
    axisY[1] = axisZ[2] * axisX[0] - axisZ[0] * axisX[2];
    axisY[2] = axisZ[0] * axisX[1] - axisZ[1] * axisX[0];

    length = sqrtf(axisY[0] * axisY[0] + axisY[1] * axisY[1] + axisY[2] * axisY[2]);

    if (length != 0.0f)
    {
        axisY[0] /= length;
        axisY[1] /= length;
        axisY[2] /= length;
    }

    memset(result, 0x0, sizeof(Matrix));

    result->m[0][0] = -axisX[0];
    result->m[0][1] = axisY[0];
    result->m[0][2] = -axisZ[0];

    result->m[1][0] = -axisX[1];
    result->m[1][1] = axisY[1];
    result->m[1][2] = -axisZ[1];

    result->m[2][0] = -axisX[2];
    result->m[2][1] = axisY[2];
    result->m[2][2] = -axisZ[2];

    result->m[3][0] = axisX[0] * posX + axisX[1] * posY + axisX[2] * posZ;
    result->m[3][1] = -axisY[0] * posX - axisY[1] * posY - axisY[2] * posZ;
    result->m[3][2] = axisZ[0] * posX + axisZ[1] * posY + axisZ[2] * posZ;
    result->m[3][3] = 1.0f;
}
