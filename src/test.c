#include <assert.h>
#include "Renderer.h"
#include "SDLGLApp.h"


void testRenderer(Renderer* r)
{
    assert(r);
    /* Assert data is loaded */
    assert(r->vertexDataSize > 0);
    assert(r->vertexData != 0);
    assert(r->numIndices > 0);
    assert(r->indices != 0);
    assert(r->numSceneNodes > 0);
    assert(r->sceneNodes != 0);
    assert(r->numMaterials > 0);
    assert(r->materials != 0);
}

void runTests(SDLGLApp* a)
{
    assert(a);
    testRenderer(&a->renderer);
}

int main(int argc, char** argv)
{
    SDLGLApp app;
    char dbFile[100];

    if(argc != 2)
    {
        Log_error("Usage: %s <file.db>\n", argv[0]);
        return 5;
    }

    dbFile[0] = '\0';
    strcat(dbFile, MODEL_DIRECTORY);
    strcat(dbFile, DIRECTORY_SEPARATOR);
    strcat(dbFile, argv[1]);
    SDLGLApp_init(&app, "portland.db");
    Log("Scene loaded\n");
    app.runLevel = -1;
    SDLGLApp_start(&app);
    Log("Scene rendered once\nRunning assertions\n");
    runTests(&app);
    SDLGLApp_destroy(&app);
    Log("Test passed\n");
    return 0;
}
