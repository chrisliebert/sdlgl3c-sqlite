#ifndef _SDLGLAPP_H_
#define _SDLGLAPP_H_

#include "Common.h"
#include "Renderer.h"

typedef struct SDLGLApp
{
    SDL_Window* window;
    Renderer renderer;
    Camera camera;
    SDL_GLContext glContext;
    SDL_Event event;
    float position[3];
    float direction[3];
    float right[3];
    float horizontalAngle;
    float verticalAngle;
    float mouseSpeed;
    float deltaTime;
    int runLevel;
    double lastTime;
} SDLGLApp;

void SDLGLApp_init(SDLGLApp*, const char*);
void SDLGLApp_start(SDLGLApp*);
void SDLGLApp_destroy(SDLGLApp*);

#endif /* _SDLGLAPP_H_ */
