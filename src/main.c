#include "SDLGLApp.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        Log_error("Usage: %s <file.db>\n", argv[0]);
        return 5;
    }
    SDLGLApp app;
    char dbFile[100];
    dbFile[0] = '\0';
    strcat(dbFile, MODEL_DIRECTORY);
    strcat(dbFile, DIRECTORY_SEPARATOR);
    strcat(dbFile, argv[1]);
    SDLGLApp_init(app, dbFile);
    SDLGLApp_start(app);
    SDLGLApp_destroy(app);
    return 0;
}
