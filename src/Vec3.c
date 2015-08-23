#include "Vec3.h"

void Vec3_add(float* result, float* a, float* b)
{
    int i;
    assert(result);
    assert(a);
    assert(b);
    for(i=0; i<3; i++)
    {
        result[i] = a[i] + b[i];
    }
}

float* Vec3_scaled(float* a, float c)
{
    int i;
    float* result = malloc(sizeof(float) * 3);
    assert(result);
    assert(a);
    for(i = 0; i<3; i++)
    {
        result[i] = a[i] * c;
    }
    return result;
}
