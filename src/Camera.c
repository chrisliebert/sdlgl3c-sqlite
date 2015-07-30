#include "Camera.h"

void Camera_init(Camera* camera)
{
    /* Get viewport */
    GLint mViewport[4];
    glGetIntegerv( GL_VIEWPORT, mViewport);

    /* Set up Projection and ModelView matrices */
    Matrix_loadIdentity(&camera->projectionMatrix);
    Matrix_perspective(&camera->projectionMatrix, 45.0f, (float) mViewport[2] / (float) mViewport[3], 0.1f, 10000.0f);

    /* setup modelview matrix (look down the negative z-axis) */
    Matrix_loadIdentity(&camera->modelViewMatrix);
    Matrix_lookAt(&camera->modelViewMatrix, 0.0f, 0.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

void Camera_destroy(Camera* camera)
{
}

void Camera_moveForward(Camera* camera, float amount)
{
    camera->modelViewMatrix.m[3][2] -= amount;
}

void Camera_moveLeft(Camera* camera, float amount)
{
    camera->modelViewMatrix.m[3][0] += amount;
}

void Camera_moveRight(Camera* camera, float amount)
{
    camera->modelViewMatrix.m[3][0] -= amount;
}
