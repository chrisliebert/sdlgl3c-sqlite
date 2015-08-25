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

EXPORT GLuint Shader_getId(Shader*);

EXPORT void FragmentShader_init(FragmentShader*, const char*);
EXPORT void FragmentShader_destroy(FragmentShader*);

EXPORT void VertexShader_init(VertexShader*, const char*);
EXPORT void VertexShader_destroy(VertexShader*);

#endif /* _SHADER_H_ */
