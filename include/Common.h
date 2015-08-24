#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef NULL
#define NULL 0
#endif

#undef __cplusplus

#define _DEBUG 1 /* TODO: integrate with cmake */

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
#include <SDL_thread.h>

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

void infoMsg(const char*);
void errorMsg(const char*);

/* Log messages with message boxes */
char _err_msg[MAX_LOG_LENGTH];

#define Log(A)				printf(A);
#define Logf(A,...)         printf(A,##__VA_ARGS__);

#ifdef ERROR_LOG_TO_CONSOLE
	#define Log_error(A)        fprintf(stderr,A);
	#define Log_errorf(A,...)   fprintf(stderr,A,##__VA_ARGS__);
#else
	#define Log_error(A)		errorMsg(A);
	#define Log_errorf(A,...)	_err_msg[0] = '\0'; sprintf(_err_msg,A,##__VA_ARGS__); errorMsg(_err_msg); _err_msg[0] = '\0';
#endif

#endif /* _COMMON_H_ */
