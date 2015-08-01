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
} Camera;

void Camera_aim(Camera* camera, float x, float y);
void Camera_init(Camera* camera);
void Camera_moveBackward(Camera* camera, float amount);
void Camera_moveForward(Camera* camera, float amount);
void Camera_moveLeft(Camera* camera, float amount);
void Camera_moveRight(Camera* camera, float amount);
void Camera_update(Camera* camera);

#endif
