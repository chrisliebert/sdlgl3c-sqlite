#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef NULL
#define NULL 0
#endif

#undef __cplusplus

#define _DEBUG 0 /* TODO: integrate with cmake */

#ifdef _DEBUG
#include <assert.h>
#else
#define assert(A) (void*)(0 * A)
#endif /* _DEBUG */

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

#define MODEL_DIRECTORY "."

/* Cross-platform directory separator */
#ifdef _WIN32
#define DIRECTORY_SEPARATOR "\\"
#else
#define DIRECTORY_SEPARATOR "/"
#endif

#define MAX_SHADER_LOG_LENGTH 1000
#define MAX_FILENAME_LENGTH 1000
#define MAX_LOG_LENGTH 1000

#if defined(_MSC_VER)
    /*  Microsoft MSVC */
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif(__MINGW32__)
	/* MinGW */
    #define EXPORT __declspec(dllexport)
    #define IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__GNUG__)
	/* Unix GCC */
	#define EXPORT
	#define IMPORT
#else
    /* Other compiler */
    #define EXPORT
    #define IMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#define Log(A)				printf("%s", A);
#define Logf(A,...)         printf(A,##__VA_ARGS__);

#define Log_error(A)        fprintf(stderr,"%s", A);
#define Log_errorf(A,...)   fprintf(stderr,A,##__VA_ARGS__);

#endif /* _COMMON_H_ */
