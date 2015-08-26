#include "SDLGLApp.h"

SDLGLApp* app;

/* Update callback */
void fly()
{
    Camera_moveForward(&app->camera, 0.1f);
}

int main(int argc, char** argv)
{
    SDLGLApp a;
    char dbFile[100];

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <file.db>\n", argv[0]);
        return 0;
    }

    app = &a; // Set global application pointer
    dbFile[0] = '\0';
    strcat(dbFile, MODEL_DIRECTORY);
    strcat(dbFile, DIRECTORY_SEPARATOR);
    strcat(dbFile, argv[1]);
    SDLGLApp_init(&a, dbFile);

    /* Set update callback (omit to disable mult-threaded callback) */
    a.updateCB = &fly;

    SDLGLApp_start(&a);
    SDLGLApp_destroy(&a);
    return 0;
}
