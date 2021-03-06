#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Camera.h"
#include "Common.h"
#include "Frustum.h"
#include "Shader.h"

void _checkForGLError(const char *file, int line);

#ifdef _DEBUG
#define checkForGLError() _checkForGLError(__FILE__,__LINE__)
#else
#define checkForGLError() (void)0
#endif

#define MAX_NAME_LENGTH 128

typedef struct
{
    char name[MAX_NAME_LENGTH];
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float transmittance[3];
    float emission[3];
    float shininess;
    float ior; /* index of refraction */
    float dissolve; /* 1 == opaque; 0 == fully transparent */
    /* illumination model (see http://www.fileformat.info/format/material/) */
    int illum;
    /* Texture file names */
    char ambientTextureName[MAX_NAME_LENGTH];
    char diffuseTextureName[MAX_NAME_LENGTH];
    char specularTextureName[MAX_NAME_LENGTH];
    char normalTextureName[MAX_NAME_LENGTH];
} Material;

typedef struct
{
    char name[MAX_NAME_LENGTH];
    int materialId;
    Matrix modelViewMatrix;
    GLuint startPosition;
    GLuint endPosition;
    GLenum primativeMode;
    GLuint ambientTextureId;
    GLuint diffuseTextureId;
    GLuint normalTextureId;
    GLuint specularTextureId;
    float boundingSphere;
    float lx, ly, lz;
} SceneNode;

typedef struct Vertex
{
    GLfloat vertex[3];
    GLfloat normal[3];
    GLfloat texcoord[2];
} Vertex;

typedef struct Renderer
{
    GLuint vao, vbo, ibo;
    GLuint gpuProgram;
    Vertex* vertexData;
    int vertexDataSize;
    SceneNode* sceneNodes;
    int numSceneNodes;
    GLuint* indices;
    int numIndices;
    int numMaterials;
    Material* materials;
    VertexShader vertShader;
    FragmentShader fragShader;
    char* dbFileName;
    bool useFixedFunctionLegacyMode; /* Disable shaders and used OpenGL 1 immediate mode */
} Renderer;

EXPORT void Renderer_bufferToGPU(Renderer*);
EXPORT void Renderer_init(Renderer*, const char*);
EXPORT void Renderer_destroy(Renderer*);
EXPORT void Renderer_buildScene(Renderer*);
EXPORT void Renderer_render(Renderer*, Camera*);

#endif
