#include "Camera.h"

void Camera_aim(Camera* camera, float x, float y)
{
    camera->horizontalAngle += x;
    camera->verticalAngle += y;

    camera->direction[0] = cos(camera->verticalAngle) * sin(camera->horizontalAngle);
    camera->direction[1] = sin(camera->verticalAngle);
    camera->direction[2] = cos(camera->verticalAngle) * cos(camera->horizontalAngle);

    /* Right vector */
    camera->right[0] = sin(camera->horizontalAngle - 3.14f / 2.0f);
    camera->right[1] = 0;
    camera->right[2] = cos(camera->horizontalAngle - 3.14f / 2.0f);

    /* Up vector */

    /* vec3 up = cross( right, direction ) */
    /* TODO: implement cross-product routine */


    Matrix_lookAt(&camera->modelViewMatrix, camera->position[0], camera->position[1], camera->position[2], camera->position[0] + camera->direction[0],
            camera->position[1] + camera->direction[1], camera->position[2] + camera->direction[2], camera->up[0], camera->up[1], camera->up[2]);
}

void Camera_init(Camera* camera)
{
    camera->position[0] = 0.f;
    camera->position[1] = 1.f;
    camera->position[2] = 5.f;
    camera->horizontalAngle = 3.14159f;
    camera->verticalAngle = 0.f;
    camera->up[0] = 0.f;
    camera->up[1] = 1.f;
    camera->up[2] = 0.f;
    camera->speed = 0.1f;
    /* Get viewport */
    GLint mViewport[4];
    glGetIntegerv( GL_VIEWPORT, mViewport);

    /* Set up Projection and ModelView matrices */
    Matrix_loadIdentity(&camera->projectionMatrix);
    Matrix_perspective(&camera->projectionMatrix, 45.0f, (float) mViewport[2] / (float) mViewport[3], 0.1f, 10000.0f);

    /* setup modelview matrix (look down the negative z-axis) */
    Matrix_loadIdentity(&camera->modelViewMatrix);
    Camera_aim(camera, 0.f, 0.f);
}

void Camera_moveBackward(Camera* camera, float amount)
{
    Camera_moveForward(camera, amount * -1.f);
}

void Camera_moveForward(Camera* camera, float amount)
{
    float* scaledDirection = Vec3_scaled(camera->direction, amount * camera->speed);
    Vec3_add(camera->position, scaledDirection, camera->position);
    free(scaledDirection);
}

void Camera_moveLeft(Camera* camera, float amount)
{
    Camera_moveRight(camera, amount * -1.f);
}

void Camera_moveRight(Camera* camera, float amount)
{
    //camera->position[0] += amount;
    //position += right * deltaTime * speed;
    float* scaledRight = Vec3_scaled(camera->right, amount  * camera->speed);
    Vec3_add(camera->position, scaledRight, camera->position);
    free(scaledRight);
}

void Camera_update(Camera* camera)
{
    Matrix_lookAt(&camera->modelViewMatrix, camera->position[0], camera->position[1], camera->position[2], camera->position[0] + camera->direction[0],
            camera->position[1] + camera->direction[1], camera->position[2] + camera->direction[2], camera->up[0], camera->up[1], camera->up[2]);
}
