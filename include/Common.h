#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef NULL
#define NULL 0
#endif

/* Fix static analysis for eclipse */
#undef __cplusplus

#define _DEBUG 1
#define USE_GLEW 1
#ifdef _WIN32
#define GLEW_STATIC 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#ifdef USE_GLEW
#include <GL/glew.h>
#else
#include <SDL_opengl.h>
#endif

#define MODEL_DIRECTORY "models"
#define TEXTURE_DIRECTORY "textures"

/* Cross-platform directory separator */
#ifdef _WIN32
#define DIRECTORY_SEPARATOR "\\"
#else
#define DIRECTORY_SEPARATOR "/"
#endif

#define MAX_SHADER_LOG_LENGTH 1000
#define MAX_FILENAME_LENGTH 1000

#define Log(A,...)  printf(A,##__VA_ARGS__);
#define Log_message(A,...)  printf(A,##__VA_ARGS__);
#define Log_error(A,...)    fprintf(stderr,A,##__VA_ARGS__);

#endif
