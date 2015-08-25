#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Common.h"
#include "Matrix.h"
#include "Vec3.h"

typedef struct Camera
{
    Matrix modelViewMatrix;
    Matrix projectionMatrix;
    float position[3];
    float direction[3];
    float right[3];
    float up[3];
    float horizontalAngle;
    float verticalAngle;
    float speed;
} Camera;

EXPORT void Camera_aim(Camera* camera, float x, float y);
EXPORT void Camera_init(Camera* camera);
EXPORT void Camera_moveBackward(Camera* camera, float amount);
EXPORT void Camera_moveForward(Camera* camera, float amount);
EXPORT void Camera_moveLeft(Camera* camera, float amount);
EXPORT void Camera_moveRight(Camera* camera, float amount);
EXPORT void Camera_update(Camera* camera);

#endif
