#include "SDLGLApp.h"


int main(int argc, char** argv) {
	SDLGLApp a;
	char dbFile[100];

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file.db>\n", argv[0]);
		return 0;
	}

	dbFile[0] = '\0';
	strcat(dbFile, MODEL_DIRECTORY);
	strcat(dbFile, DIRECTORY_SEPARATOR);
	strcat(dbFile, argv[1]);
	SDLGLApp_init(&a, dbFile);

	while (a.runLevel > 0) {
		SDLGLApp_update(&a);
	}

	SDLGLApp_destroy(&a);
	return 0;
}
