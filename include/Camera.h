#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Common.h"
#include "Matrix.h"

typedef struct Camera
{
    Matrix modelViewMatrix;
    Matrix projectionMatrix;
} Camera;

void Camera_init(Camera* camera);
void Camera_moveForward(Camera* camera, float amount);
void Camera_moveLeft(Camera* camera, float amount);
void Camera_moveRight(Camera* camera, float amount);

#endif
