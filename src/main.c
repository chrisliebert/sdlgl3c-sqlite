#include "SDLGLApp.h"

int main(int argc, char** argv)
{
    SDLGLApp app;
    char dbFile[100];

    if(argc != 2)
    {
        Log_errorf("Usage: %s <file.db>\n", argv[0]);
        return 0;
    }

    dbFile[0] = '\0';
    strcat(dbFile, MODEL_DIRECTORY);
    strcat(dbFile, DIRECTORY_SEPARATOR);
    strcat(dbFile, argv[1]);
    SDLGLApp_init(&app, dbFile);
    SDLGLApp_start(&app);
    SDLGLApp_destroy(&app);
    return 0;
}
