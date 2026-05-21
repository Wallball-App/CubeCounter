#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Cube.h"

using namespace glm;
using namespace std;

class Array {
    public:
        enum FaceIndex {
            FRONT,
            BACK,
            LEFT,
            RIGHT,
            TOP,
            BOTTOM,
            NONE
        };
        struct Plane{
            vec3 position;
            vec3 rotation;
            vec3 scale;
            
            FaceIndex Face;
            int RandomID;
        };
        
        int cubesize;
        mat4* viewMat;
        mat4* projectionMat;

        float speed = 0;
        float FOV = 45.0f;
        Array();
        Array(int size, mat4 &view, mat4 &projection, bool scrambled);
        void ClonePlanes(int size, mat4 &view, mat4 &projection);
        void CreateFace(std::vector<Plane> &planevector, int size, float planeSize, vec3 scale, float offset, vec3 pos, vec3 axis, FaceIndex face);
        void render(mat4 &view, mat4 &projection);
        void SetupMatrices(mat4 view, mat4 projection, vec3 position);
        void GenerateArray();
        void bind();
        void Destroy();
        void SetVars(int size, bool scrambled);
    private:
        
        std::string loadShader(const char *path);
        
        // Shader source strings (must persist)
        std::string vertexShaderStr;
        std::string fragmentShaderStr;
};