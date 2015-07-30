#include "Frustum.h"

void Frustum_destroy(Frustum* frustum)
{
    free(frustum);
}

void Frustum_init(Frustum* frustum)
{
    frustum = malloc(sizeof(Frustum));
}

void Frustum_extractFrustum(Frustum* frustum, Matrix* modelViewMatrix, Matrix* projectionMatrix)
{
    float *modl = Matrix_getOneDimensionalArray(modelViewMatrix);
    float *proj = Matrix_getOneDimensionalArray(projectionMatrix);

    float clip[16];
    float t;

    /* Combine the two matrices (multiply projection by modelview) */
    clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
    clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
    clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
    clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

    clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
    clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
    clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
    clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

    clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
    clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
    clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
    clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

    clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
    clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
    clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
    clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

    /* Extract the numbers for the RIGHT plane */
    frustum->f[0][0] = clip[3] - clip[0];
    frustum->f[0][1] = clip[7] - clip[4];
    frustum->f[0][2] = clip[11] - clip[8];
    frustum->f[0][3] = clip[15] - clip[12];

    /* Normalize the result */
    t = sqrt(frustum->f[0][0] * frustum->f[0][0] + frustum->f[0][1] * frustum->f[0][1] + frustum->f[0][2] * frustum->f[0][2]);
    frustum->f[0][0] /= t;
    frustum->f[0][1] /= t;
    frustum->f[0][2] /= t;
    frustum->f[0][3] /= t;

    /* Extract the numbers for the LEFT plane */
    frustum->f[1][0] = clip[3] + clip[0];
    frustum->f[1][1] = clip[7] + clip[4];
    frustum->f[1][2] = clip[11] + clip[8];
    frustum->f[1][3] = clip[15] + clip[12];

    /* Normalize the result */
    t = sqrt(frustum->f[1][0] * frustum->f[1][0] + frustum->f[1][1] * frustum->f[1][1] + frustum->f[1][2] * frustum->f[1][2]);
    frustum->f[1][0] /= t;
    frustum->f[1][1] /= t;
    frustum->f[1][2] /= t;
    frustum->f[1][3] /= t;

    /* Extract the BOTTOM plane */
    frustum->f[2][0] = clip[3] + clip[1];
    frustum->f[2][1] = clip[7] + clip[5];
    frustum->f[2][2] = clip[11] + clip[9];
    frustum->f[2][3] = clip[15] + clip[13];

    /* Normalize the result */
    t = sqrt(frustum->f[2][0] * frustum->f[2][0] + frustum->f[2][1] * frustum->f[2][1] + frustum->f[2][2] * frustum->f[2][2]);
    frustum->f[2][0] /= t;
    frustum->f[2][1] /= t;
    frustum->f[2][2] /= t;
    frustum->f[2][3] /= t;

    /* Extract the TOP plane */
    frustum->f[3][0] = clip[3] - clip[1];
    frustum->f[3][1] = clip[7] - clip[5];
    frustum->f[3][2] = clip[11] - clip[9];
    frustum->f[3][3] = clip[15] - clip[13];

    /* Normalize the result */
    t = sqrt(frustum->f[3][0] * frustum->f[3][0] + frustum->f[3][1] * frustum->f[3][1] + frustum->f[3][2] * frustum->f[3][2]);
    frustum->f[3][0] /= t;
    frustum->f[3][1] /= t;
    frustum->f[3][2] /= t;
    frustum->f[3][3] /= t;

    /* Extract the FAR plane */
    frustum->f[4][0] = clip[3] - clip[2];
    frustum->f[4][1] = clip[7] - clip[6];
    frustum->f[4][2] = clip[11] - clip[10];
    frustum->f[4][3] = clip[15] - clip[14];

    /* Normalize the result */
    t = sqrt(frustum->f[4][0] * frustum->f[4][0] + frustum->f[4][1] * frustum->f[4][1] + frustum->f[4][2] * frustum->f[4][2]);
    frustum->f[4][0] /= t;
    frustum->f[4][1] /= t;
    frustum->f[4][2] /= t;
    frustum->f[4][3] /= t;

    /* Extract the NEAR plane */
    frustum->f[5][0] = clip[3] + clip[2];
    frustum->f[5][1] = clip[7] + clip[6];
    frustum->f[5][2] = clip[11] + clip[10];
    frustum->f[5][3] = clip[15] + clip[14];

    /* Normalize the result */
    t = sqrt(frustum->f[5][0] * frustum->f[5][0] + frustum->f[5][1] * frustum->f[5][1] + frustum->f[5][2] * frustum->f[5][2]);
    frustum->f[5][0] /= t;
    frustum->f[5][1] /= t;
    frustum->f[5][2] /= t;
    frustum->f[5][3] /= t;
}

bool Frustum_pointInFrustum(Frustum* frustum, float x, float y, float z)
{
    int p;

    for (p = 0; p < 6; p++)
        if (frustum->f[p][0] * x + frustum->f[p][1] * y + frustum->f[p][2] * z + frustum->f[p][3] <= 0)
            return (false);
    return (true);
}

bool Frustum_sphereInFrustum(Frustum* frustum, float x, float y, float z, float radius)
{
    int p;

    for (p = 0; p < 6; p++)
        if (frustum->f[p][0] * x + frustum->f[p][1] * y + frustum->f[p][2] * z + frustum->f[p][3] <= -radius)
            return (false);
    return (true);
}

float Frustum_sphereInFrustumDistance(Frustum* frustum, float x, float y, float z, float radius)
{
    int p;
    float d;

    for (p = 0; p < 6; p++)
    {
        d = frustum->f[p][0] * x + frustum->f[p][1] * y + frustum->f[p][2] * z + frustum->f[p][3];
        if (d <= -radius)
            return (0);
    }
    return (d + radius);
}

bool Frustum_cubeInFrustum(Frustum* frustum, float x, float y, float z, float size)
{
    int p;

    for (p = 0; p < 6; p++)
    {
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            continue;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            continue;
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            continue;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            continue;
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            continue;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            continue;
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            continue;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            continue;
        return (false);
    }
    return (true);
}

/* Returns 0 if the sphere is totally outside, 1 if it's partially inside, and 2 if it's totally inside */
int Frustum_spherePartiallyInFrustum(Frustum* frustum, float x, float y, float z, float radius)
{
    int p;
    int c = 0;
    float d;

    for (p = 0; p < 6; p++)
    {
        d = frustum->f[p][0] * x + frustum->f[p][1] * y + frustum->f[p][2] * z + frustum->f[p][3];
        if (d <= -radius)
            return (0);
        if (d > radius)
            c++;
    }
    return ((c == 6) ? 2 : 1);
}

/* Returns 0 if the cube is totally outside, 1 if it's partially inside, and 2 if it's totally inside */
int Frustum_cubePartiallyInFrustum(Frustum* frustum, float x, float y, float z, float size)
{
    int p;
    int c;
    int c2 = 0;

    for (p = 0; p < 6; p++)
    {
        c = 0;
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            c++;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            c++;
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            c++;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z - size) + frustum->f[p][3] > 0)
            c++;
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            c++;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y - size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            c++;
        if (frustum->f[p][0] * (x - size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            c++;
        if (frustum->f[p][0] * (x + size) + frustum->f[p][1] * (y + size) + frustum->f[p][2] * (z + size) + frustum->f[p][3] > 0)
            c++;
        if (c == 0)
            return (0);
        if (c == 8)
            c2++;
    }
    return ((c2 == 6) ? 2 : 1);
}

bool Frustum_polygonInFrustum(Frustum* frustum, int numpoints, Point* pointlist)
{
    int f, p;

    for (f = 0; f < 6; f++)
    {
        for (p = 0; p < numpoints; p++)
        {
            if (frustum->f[f][0] * pointlist[p].x + frustum->f[f][1] * pointlist[p].y + frustum->f[f][2] * pointlist[p].z + frustum->f[f][3] > 0)
                break;
        }
        if (p == numpoints)
            return (false);
    }
    return (true);
}

