#ifndef _SHADER_H_
#define _SHADER_H_

#include "Common.h"

typedef struct Shader
{
    GLuint id;
    const char* filePath;
    const char* shaderSrc;
} Shader;

typedef struct FragmentShader
{
    GLuint id;
    const char* filePath;
    const char* shaderSrc;
} FragmentShader;

typedef struct VertexShader
{
    GLuint id;
    const char* filePath;
    const char* shaderSrc;
} VertexShader;

GLuint Shader_getId(Shader*);

void FragmentShader_init(FragmentShader*, const char*);
void FragmentShader_destroy(FragmentShader*);

void VertexShader_init(VertexShader*, const char*);
void VertexShader_destroy(VertexShader*);

#endif /* _SHADER_H_ */
