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

EXPORT void Frustum_init(Frustum*);
EXPORT void Frustum_destroy(Frustum*);
EXPORT void Frustum_extractFrustum(Frustum* frustum, Matrix* modelViewMatrix, Matrix* projectionMatrix);
EXPORT bool Frustum_pointInFrustum(Frustum* frustum, float x, float y, float z);
EXPORT bool Frustum_sphereInFrustum(Frustum* frustum, float x, float y, float z, float radius);
EXPORT float Frustum_sphereInFrustumDistance(Frustum* frustum, float x, float y, float z, float radius);
EXPORT int Frustum_spherePartiallyInFrustum(Frustum* frustum, float x, float y, float z, float radius);
EXPORT bool Frustum_cubeInFrustum(Frustum* frustum, float x, float y, float z, float size);
EXPORT int Frustum_cubePartiallyInFrustum(Frustum* frustum, float x, float y, float z, float size);
EXPORT bool Frustum_polygonInFrustum(Frustum* frustum, int numpoints, Point* pointlist);

#endif /* _FRUSTUM_H_ */
