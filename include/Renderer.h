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

typedef struct
{
    char name[64];
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
    char ambient_texture_name[64];
    char diffuse_texture_name[64];
    char specular_texture_name[64];
    char normal_texture_name[64];
} Material;

typedef struct
{
    char name[64];
    int material_id;
    Matrix model_view_matrix;
    GLuint start_position;
    GLuint end_position;
    GLenum primative_mode;
    GLuint ambient_texture_id;
    GLuint diffuse_texture_id;
    GLuint normal_texture_id;
    GLuint specular_texture_id;
    GLfloat bounding_sphere;
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
	bool useFixedFunctionLegacyMode; // Disable shaders and used OpenGL 1 immediate mode
} Renderer;

void Renderer_bufferToGPU(Renderer*);
void Renderer_init(Renderer*, const char*);
void Renderer_destroy(Renderer*);
void Renderer_buildScene(Renderer*);
void Renderer_render(Renderer*, Camera*);

#endif
