#ifndef _SCENE_BUILDER_H_
#define _SCENE_BUILDER_H_

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "tiny_obj_loader.h"

#define MODEL_DIRECTORY "models"
#define TEXTURE_DIRECTORY "textures"

// Cross-platform directory separator
#ifdef _WIN32
#define DIRECTORY_SEPARATOR "\\"
#else
#define DIRECTORY_SEPARATOR "/"
#endif

using std::cout;
using std::cerr;
using std::endl;
using std::string;

typedef struct
{
    float vertex[3];
    float normal[3];
    float textureCoordinate[2];
} Vertex;

typedef struct
{
    // Material name
    char name[64];
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float transmittance[3];
    float emission[3];
    float shininess;
    float ior;      // index of refraction
    float dissolve; // 1 == opaque; 0 == fully transparent
    // illumination model (see http://www.fileformat.info/format/material/)
    int illum;
    //Texture file names
    char ambientTexName[64];
    char diffuseTexName[64];
    char specularTexName[64];
    char normalTexName[64];
} Material;


typedef struct
{
    std::string name;
    std::string material;
    Vertex* vertexData;
    size_t vertexDataSize;
    glm::mat4 modelViewMatrix;
    unsigned startPosition;
    unsigned endPosition;
    int primativeMode;
    unsigned ambientTextureId;
    unsigned diffuseTextureId;
    unsigned normalTextureId;
    unsigned specularTextureId;
    float boundingSphere;
	float lx, ly, lz; // position of center
} SceneNode;


class SceneBuilder
{
public:
    SceneBuilder();
    ~SceneBuilder();
    void addMaterial(Material*);
    void addSceneNode(SceneNode*);
    void addTexture(const char*, unsigned&);
    void addWavefront(const char*, glm::mat4);
    void buildScene();
    void saveToDB(const char*);
    int getMaterialId(string&);
private:
    unsigned startPosition;
    glm::mat4 modelViewProjectionMatrix;
    std::vector<Vertex> vertexData;
    std::vector<SceneNode> sceneNodes;
    std::vector<unsigned> indices;
    std::map<string, Material> materials;
    std::map<std::string, int> textures;
};

#endif //_SCENE_BUILDER_H_
