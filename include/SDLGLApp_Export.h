#ifndef _SDLGLAPP_H_
#define _SDLGLAPP_H_

#include "Renderer.h"

//#ifdef WINDOWS
//#define DLL __declspec(dllexport)
//#else
//#endif

#if defined(_MSC_VER)
    //  Microsoft
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(_GCC)
    //  GCC

        #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)

    #define EXPORT __attribute__((visibility("default")))
    #define IMPORT
#else
    //  do nothing and hope for the best?
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

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
EXPORT void SDLGLApp_start(SDLGLApp*);
EXPORT void SDLGLApp_destroy(SDLGLApp*);

#endif /* _SDLGLAPP_H_ */
