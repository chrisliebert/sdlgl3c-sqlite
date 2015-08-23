#include "SDLGLApp.h"

void infoMsg(const char* msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info", msg, NULL);
}

void errorMsg(const char* title)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, SDL_GetError(), NULL);
}

void SDLGLApp_init(SDLGLApp* app, const char* dbFileName)
{
    app->mouseSpeed = 0.001f;
    app->runLevel = 1;
    app->lastTime = SDL_GetTicks();
    app->deltaTime = 0;
    app->window = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        Log_error("Unable to initialize SDL: %s\n", SDL_GetError());
        app->runLevel = 0;
    }
    else
    {

        /* Manually set the OpenGL context version
         SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
         SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
         */
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
        Renderer_init(&app->renderer, dbFileName);

        Uint32 flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN; /* | SDL_WINDOW_FULLSCREEN; */
        app->window = SDL_CreateWindow("", 300, 100, 800, 600, flags);
        if (app->window == NULL)
        {
            fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
            errorMsg("Unable to create window");
            app->runLevel = 0;
            return;
        }
        else
        {
            app->glContext = SDL_GL_CreateContext(app->window);
            if (app->glContext == NULL)
            {
                errorMsg("Unable to create OpenGL context");
                errorMsg(SDL_GetError());
                app->runLevel = 0;
                return;
            }
            /* This makes our buffer swap syncronized with the monitor's vertical refresh (disabled for benchmarking) */
            SDL_GL_SetSwapInterval(0);

            /* Initialize GLEW */
#ifdef USE_GLEW
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            checkForGLError();
            if (GLEW_OK != err)
            {
                Log_error("Error: %s\n", glewGetErrorString(err));
                const char* errorStr = (char*) glewGetErrorString(err);
                errorMsg(errorStr);
                free((void*)errorStr);
            }
#endif
            Renderer_buildScene(&app->renderer);
            Renderer_bufferToGPU(&app->renderer);
            Log("OpenGL %s\n", glGetString(GL_VERSION));

            int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
            int initted = IMG_Init(flags);
            if ((initted & flags) != flags)
            {
                Log_error("IMG_Init: Failed to init required jpg and png support!\n");
                Log_error("IMG_Init: %s\n", IMG_GetError());
            }

        }

        /* Lock mouse */
        SDL_SetWindowGrab(app->window, SDL_TRUE);
        if (SDL_ShowCursor(SDL_DISABLE) < 0)
        {
            Log_error("Unable to hide the cursor");
        }

        /*
         if(SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
         errorMsg(SDL_GetError());
         }
         */

        checkForGLError();

        /* Get largest anisotropic filtering level */
        GLfloat fLargest;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
        checkForGLError();
        glEnable(GL_DEPTH_TEST);
        checkForGLError();
        /* Accept fragment if it closer to the camera than the former one */
        glDepthFunc(GL_LESS);
        glDisable(GL_CULL_FACE);
        checkForGLError();
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Camera_init(&app->camera);
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glViewport(0, 0, viewport[2], viewport[3]);
        SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
        SDL_WarpMouseInWindow(app->window, viewport[2] / 2, viewport[3] / 2);
        SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
    }
}

void SDLGLApp_destroy(SDLGLApp* app)
{
    SDL_GL_DeleteContext(app->glContext);
    SDL_DestroyWindow(app->window);
    Renderer_destroy(&app->renderer);
    IMG_Quit();
    SDL_Quit();
}

void SDLGLApp_keyDown(SDLGLApp* app, SDL_Keycode key)
{
    switch (key)
    {
    case SDLK_ESCAPE:
        app->runLevel = 0;
        break;
    }
}

void SDLGLApp_keyUp(SDLGLApp* app, SDL_Keycode key)
{

}

void SDLGLApp_start(SDLGLApp* app)
{
    while (app->runLevel > 0)
    {
        SDL_PollEvent(&app->event);

        if (app->event.type == SDL_QUIT)
        {
            app->runLevel = 0;
            break;
        }
        else if (app->event.type == SDL_KEYDOWN)
        {
            SDLGLApp_keyDown(app, app->event.key.keysym.sym);
            if (app->runLevel < 1)
            {
                break;
            }
        }
        else if (app->event.type == SDL_KEYUP)
        {
            SDLGLApp_keyUp(app, app->event.key.keysym.sym);
        }

        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W])
        {
            Camera_moveForward(&app->camera, app->deltaTime);
        }

        if (keys[SDL_SCANCODE_S])
        {
            Camera_moveBackward(&app->camera, app->deltaTime);
        }

        if (keys[SDL_SCANCODE_D])
        {
            Camera_moveRight(&app->camera, app->deltaTime);
        }

        if (keys[SDL_SCANCODE_A])
        {
            Camera_moveLeft(&app->camera, app->deltaTime);
        }

        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        double width = (double)viewport[2];
        double height = (double)viewport[3];

        /* Get mouse position */
        double xpos, ypos;
        int x, y;
        SDL_GetMouseState(&x, &y);


        // Ignore mouse input less than 2 pixels from origin (smoothing)
        if(abs(x - (int)floor(viewport[2]/2.0)) < 2) x = (int)floor(viewport[2]/2.0);
        if(abs(y - (int)floor(viewport[3]/2.0)) < 2) y = (int)floor(viewport[3]/2.0);

        //if(abs(x - viewport[2]) < 3) x = floor(width/2.0);
        //if(abs(y - viewport[3]) < 3) y = floor(height/2.0);

        xpos = (double) x;
        ypos = (double) y;

        SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
        SDL_WarpMouseInWindow(app->window, (int)(width / 2.0),(int)(height / 2.0));
        SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);

        /* Compute time difference between current and last frame */
        double currentTime = SDL_GetTicks();
        app->deltaTime = (float) (currentTime - app->lastTime);

        Camera_aim(&app->camera, app->mouseSpeed * (float) (floor(width / 2.0) - xpos), app->mouseSpeed * (float) (floor(height / 2.0) - ypos));

        Camera_update(&app->camera);
        app->lastTime = currentTime;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Renderer_render(&app->renderer, &app->camera);
        SDL_GL_SwapWindow(app->window);
    }
}
