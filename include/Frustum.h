#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "Common.h"
#include "Matrix.h"

typedef struct
{
    float x, y, z;
} Point;

typedef struct Frustum
{
    float f[6][4];
} Frustum;

void Frustum_init(Frustum*);
void Frustum_destroy(Frustum*);
void Frustum_extractFrustum(Frustum* frustum, Matrix* modelViewMatrix, Matrix* projectionMatrix);
bool Frustum_pointInFrustum(Frustum* frustum, float x, float y, float z);
bool Frustum_sphereInFrustum(Frustum* frustum, float x, float y, float z, float radius);
float Frustum_sphereInFrustumDistance(Frustum* frustum, float x, float y, float z, float radius);
int Frustum_spherePartiallyInFrustum(Frustum* frustum, float x, float y, float z, float radius);
bool Frustum_cubeInFrustum(Frustum* frustum, float x, float y, float z, float size);
int Frustum_cubePartiallyInFrustum(Frustum* frustum, float x, float y, float z, float size);
bool Frustum_polygonInFrustum(Frustum* frustum, int numpoints, Point* pointlist);

#endif /* _FRUSTUM_H_ */
