#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>
#include <SDL3/SDL_opengl.h>

#include <fstream>
#include <filesystem>

#include "Array.h"

using namespace glm;
using namespace std;

float offset = 0.5f;
const float planesize = 1.0f;
bool scrambled = false;

float vertices[] = { //Faces Camera Looking at -Z
    -planesize/2.0f, -planesize/2.0f, 0.0f, // Bottom-left
     planesize/2.0f, -planesize/2.0f, 0.0f, // Bottom-right
     planesize/2.0f,  planesize/2.0f, 0.0f, // Top-right
    -planesize/2.0f,  planesize/2.0f, 0.0f  // Top-left
};

unsigned int indices[] = {
    0, 1, 2, 
    2, 3, 0
};
unsigned int VBO, VAO, EBO;
unsigned int instanceVBO;
unsigned int shaderProgram, vertex, fragment;

glm::mat4 modelMatrix = mat4(1.0f);
vector<Array::Plane> planes;

vector<vec4> colors = {
    
    vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
    vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
    
    vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
    vec4(1.0f, 0.5f, 0.0f, 1.0f),  // Orange

    vec4(1.0f, 1.0f, 1.0f, 1.0f), // White
    vec4(1.0f, 1.0f, 0.0f, 1.0f), // Yellow
    
    vec4(0.0f, 0.0f, 0.0f, 1.0f) //Default
};

Array::Array() {
    // Default constructor (not used in this example)
}
Array::Array(int size, glm::mat4 &view, glm::mat4 &projection, bool scrambled) {
    // Store matrices as member variables
    viewMat = &view;
    projectionMat = &projection;
    srand(static_cast<unsigned int>(time(0)));
    ClonePlanes(size, view, projection);
    bind();
}
void Array::ClonePlanes(int size, glm::mat4 &view, glm::mat4 &projection) {
    /*for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            for(int k = 0; k < size; k++) {
                vec3 PlanePos = position + vec3(i + offset + (i * planesize/4.0f), j + offset + (j * planesize/4.0f), k + offset + (k * planesize/4.0f));

                
                bool onSurface = (i == 0 || i == size-1) || (j == 0 || j == size-1) || (k == 0 || k == size-1);
                if(!onSurface) continue; // Skip inner planes, only keep surface planes

                vec3 rot = vec3(0.0f); // No rotation for now, can be modified for different orientations
                if(i==0) {
                    rot.y = 90.0f; // Rotate planes on Y-axis for X=0 to face inward
                    PlanePos.x += planesize/2.0f; // Move plane back by half its size to align with grid
                }
                else if(i == size - 1) {
                    rot.y = -90.0f; // Rotate planes on Y-axis for X=size-1 to face inward
                    PlanePos.x -= planesize/2.0f; // Move plane forward by half its size to align with grid
                }

                else if(j == 0) {
                    rot.x = 90.0f; // Rotate planes on Z-axis for Y=0 to face inward
                    PlanePos.y += planesize/2.0f;
                }
                else if(j == size - 1) {
                    rot.x = -90.0f; // Rotate planes on Z-axis for Y=size-1 to face inward
                    PlanePos.y -= planesize/2.0f;
                }

                else if(k == 0) {
                    rot.y = 0.0f; // Rotate planes
                    PlanePos.z -= planesize/2.0f; // Move plane forward by half its size to align with grid
                }
                else if(k == size-1) {
                    rot.y = 0.0f; // Rotate planes
                    PlanePos.z += planesize/2.0f; // Move plane back by half its size to align with grid
                }

                Plane instance = Plane{PlanePos, rot};
                planes.push_back(instance);
            }
        }
    }*/
    cubesize = size;
    speed = 0.1f * (size / 10.0f);
    *projectionMat = perspective(radians(FOV), 1152.0f / 648.0f, 0.1f, size * 10.0f);
   if(size > 2048) {
        modelMatrix = scale(modelMatrix, vec3(size, size, size));
        size = 1;
        scrambled = false;
   } else {

   }
   float sideLength = (size * planesize + (size-1) * offset);
   float half = sideLength / 2.0f;
   float sizeOffset = ((planesize*2.0f) + (offset/2.0f))*(size/10.0f) + (offset * (0.05f*size - 0.5f));

   //float center = 
    CreateFace(planes, size, planesize, vec3(1.0f), offset, vec3(0.0f, 0.0f, -sizeOffset), vec3(0.0f, 0.0f, 1.0f), FRONT); //FRONT
    
    CreateFace(planes, size, planesize, vec3(1.0f), offset, vec3(0.0f, 0.0f, -sideLength), vec3(0.0f, 0.0f, -1.0f), BACK); //BACK

    CreateFace(planes, size, planesize, vec3(1.0f), offset, vec3(sideLength - sizeOffset, 0.0f, -sideLength), vec3(1.0f, 0.0f, 0.0f), LEFT); //LEFT
    CreateFace(planes, size, planesize, vec3(1.0f), offset, vec3(0.0f, 0.0f, -sideLength), vec3(-1.0f, 0.0f, 0.0f), RIGHT); //RIGHT
    
    CreateFace(planes, size, planesize, vec3(1.0f), offset, vec3(0.0f, sideLength - sizeOffset, -sideLength), vec3(0.0f, 1.0f, 0.0f), TOP); //TOP
    CreateFace(planes, size, planesize, vec3(1.0f), offset, vec3(0.0f, 0.0f, -sideLength), vec3(0.0f, -1.0f, 0.0f), BOTTOM); //BOTTOM
}
void Array::CreateFace(std::vector<Array::Plane> &planevector, int size, float planeSize, vec3 scale, float offset, vec3 pos, vec3 axis, Array::FaceIndex face) {
    vec3 rotation;
    Array::Plane background;
    if(axis == vec3(1.0f, 0.0f, 0.0f) || axis == vec3(-1.0f, 0.0f, 0.0f)) {
        rotation = vec3(0.0f, 90.0f, 0.0f);
    } else if(axis == vec3(0.0f, 1.0f, 0.0f) || axis == vec3(0.0f, -1.0f, 0.0f)) {
        rotation = vec3(90.0f, 0.0f, 0.0f);
    } else if(axis == vec3(0.0f, 0.0f, 1.0f) || axis == vec3(0.0f, 0.0f, -1.0f)) {
        rotation = vec3(0.0f, 0.0f, 0.0f); //No Z-Rotation Required
    } else {
        cerr<<"Invalid Axis"<<endl;
    }
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            vec3 PlanePos = vec3(0.0f);
            float x = i + offset + (i * planeSize/4.0f);
            float y = j + offset + (j * planeSize/4.0f);
            if(axis == vec3(1.0f, 0.0f, 0.0f) || axis == vec3(-1.0f, 0.0f, 0.0f)) {
                PlanePos = pos + vec3(0.0f, x, y);
            }  else if(axis == vec3(0.0f, 1.0f, 0.0f) || axis == vec3(0.0f, -1.0f, 0.0f)) {
                PlanePos = pos + vec3(x, 0.0f, y);
            } else if(axis == vec3(0.0f, 0.0f, 1.0f) || axis == vec3(0.0f, 0.0f, -1.0f)) {
                PlanePos = pos + vec3(x, y, 0.0f);
            } else {
                cerr<<"Invalid Axis"<<endl;
            }
            Array::Plane instance = Array::Plane{PlanePos, rotation, scale, face, rand()};

            if(i != 0 && i != size-1 && j != 0 && j != size-1) background = Array::Plane{PlanePos - axis / 100.0f, rotation, vec3(2.0f*offset + planeSize), NONE, 1};
            else background = background = Array::Plane{PlanePos - axis / 100.0f, rotation, vec3(offset + planeSize/2.0f), NONE, 1};
            
            planevector.push_back(instance);
            if(size < 150) planevector.push_back(background);
        }
    }
}
void Array::bind() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Plane), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, planes.size() * sizeof(Plane), planes.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Plane), (void*)0); //POSITION
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1); // Update instance data per instance

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Plane), (void*)(sizeof(glm::vec3))); //ROTATION
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1); // Update instance data per instance

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Plane), (void*)(2*sizeof(glm::vec3))); //SCALE
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1); // Update instance data per instance




    glVertexAttribIPointer(4, 1, GL_INT, sizeof(Plane), (void*)offsetof(Array::Plane, RandomID)); //COLOR
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1); // Update instance data per instance

    glVertexAttribIPointer(5, 1, GL_INT, sizeof(Plane), (void*)offsetof(Array::Plane, Face)); //FACE
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1); // Update instance data per instance


    glBindVertexArray(0);

    // Build full path to shaders (relative to executable location)
    
    // Load into member variables so they persist
    vertexShaderStr = loadShader("../shaders/vertex.glsl");
    if(vertexShaderStr == "") vertexShaderStr = loadShader("shaders/vertex.glsl");
    fragmentShaderStr = loadShader("../shaders/fragment.glsl");
    if(fragmentShaderStr == "") fragmentShaderStr = loadShader("shaders/fragment.glsl");

    const char* vertexShaderCode = vertexShaderStr.c_str();
    const char* fragmentShaderCode = fragmentShaderStr.c_str();
    GLint success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cerr << "Vertex shader compilation failed: " << infoLog << endl;
    }


    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);


    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cerr << "Fragment shader compilation failed: " << infoLog << endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex); 
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Array::render(mat4 &view, mat4 &projection) {
    viewMat = &view;
    projectionMat = &projection;
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glUseProgram(shaderProgram);
    
    // Send matrices to shader
    //modelMatrix = rotate(modelMatrix, radians(0.5f), vec3(0.0f, 1.0f, 0.0f));

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    int colorLoc = glGetUniformLocation(shaderProgram, "colors");
    int scrambledLoc = glGetUniformLocation(shaderProgram, "scrambled");
    int randoffsetLoc = glGetUniformLocation(shaderProgram, "randoffset");
    glUniform4fv(colorLoc, 7, &colors[0].r);
    glUniform1i(scrambledLoc, scrambled);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, planes.size());
    glBindVertexArray(0);
}
void Array::Destroy() {
    planes.clear();
    modelMatrix = mat4(1.0f);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteShader(shaderProgram);
}
void Array::SetVars(int size, bool cubescrambled) {
    Array::Destroy();
    scrambled = cubescrambled;
    srand(static_cast<unsigned int>(time(0)));
    ClonePlanes(size, *viewMat, *projectionMat);
    bind();
}
string Array::loadShader(const char *path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open shader file: " << path << endl;
        return "";
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    cout<<"Shader file loaded successfully at: " << path <<endl;
    file.close();
    return content;
}