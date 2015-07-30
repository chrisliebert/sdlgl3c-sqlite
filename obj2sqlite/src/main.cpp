#include "SceneBuilder.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " objfile.obj" << endl;
        return -1;
    }
    if(strlen(argv[1]) < 5)
    {
        cerr << "Must end in .obj" << endl;
        return -2;
    }
    string inputFileName(argv[1]);
    string outFileName(inputFileName.substr(0, strlen(argv[1]) - 4) + ".db");
    SceneBuilder sceneBuilder;
    sceneBuilder.addWavefront(argv[1], glm::translate(glm::mat4(1.f), glm::vec3(0.0,0.0, 0.0)));
    sceneBuilder.buildScene();
    sceneBuilder.saveToDB(outFileName.c_str());
    return 0;
}
