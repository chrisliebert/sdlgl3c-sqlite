#ifndef _VEC3_H_
#define _VEC3_H_

#include "Common.h"

void Vec3_add(float* result, float* a, float* b);

/* Methods ending in 'd' will return a pointer to memory that is allocated and must be freed after use */
float* Vec3_scaled(float* a, float c);

#endif /* _VEC3_H_ */
