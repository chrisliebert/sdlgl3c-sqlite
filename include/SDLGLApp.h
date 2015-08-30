#ifndef _SDLGLAPP_H_
#define _SDLGLAPP_H_

#include "Common.h"
#include "Renderer.h"

EXPORT typedef struct SDLGLApp
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

EXPORT void SDLGLApp_init(SDLGLApp*, const char*);
EXPORT void SDLGLApp_update(SDLGLApp*);
EXPORT void SDLGLApp_destroy(SDLGLApp*);

EXPORT void infoMsg(const char*);
EXPORT void errorMsg(const char*);

#endif /* _SDLGLAPP_H_ */
