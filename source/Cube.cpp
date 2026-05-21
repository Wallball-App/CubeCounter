/*#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <iostream>
#include <SDL3/SDL_opengl.h>

#include <fstream>
#include <filesystem>

#include "Cube.h"

using namespace glm;
using namespace std;
using namespace filesystem;

// Get the directory where the executable is located
string getExeDir() {
    return filesystem::current_path().string();
}

const char *vertexShaderSource = "../shaders/vertex.glsl";
const char *fragmentShaderSource = "../shaders/fragment.glsl";

float vertices[] = {
    // Front face
    -0.5f, -0.5f,  0.5f,  0,
     0.5f, -0.5f,  0.5f,  0,
     0.5f,  0.5f,  0.5f,  0,
    -0.5f,  0.5f,  0.5f,  0,
    // Back face
    -0.5f, -0.5f, -0.5f,  1,
     0.5f, -0.5f, -0.5f,  1,
     0.5f,  0.5f, -0.5f,  1,
    -0.5f,  0.5f, -0.5f,  1,

    //Left face
    -0.5f, -0.5f, -0.5f,  2,
    -0.5f, -0.5f,  0.5f,  2,
    -0.5f,  0.5f,  0.5f,  2,
    -0.5f,  0.5f, -0.5f,  2,

    //Right face
     0.5f, -0.5f, -0.5f,  3,
     0.5f, -0.5f,  0.5f,  3,
     0.5f,  0.5f,  0.5f,  3,
     0.5f,  0.5f, -0.5f,  3,

    //Top face
    -0.5f,  0.5f, -0.5f,  4,
    -0.5f,  0.5f,  0.5f,  4,
    0.5f,  0.5f,  0.5f,  4,
    0.5f,  0.5f, -0.5f,  4,

    //Bottom face
    -0.5f, -0.5f, -0.5f,  5,
    -0.5f, -0.5f,  0.5f,  5,
     0.5f, -0.5f,  0.5f,  5,
     0.5f, -0.5f, -0.5f,  5
};
unsigned int indices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Back face
    4, 5, 6,
    6, 7, 4,
    // Left face
    8, 9, 10,
    10, 11, 8,
    // Right face
    12, 13, 14,
    14, 15, 12,
    // Top face
    16, 17, 18,
    18, 19, 16,
    // Bottom face
    20, 21, 22,
    22, 23, 20
};
vector<vec4> colors = {
    vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
    vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
    vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
    vec4(1.0f, 1.0f, 0.0f, 1.0f), // Yellow
    vec4(1.0f, 1.0f, 1.0f, 1.0f), // White
    vec4(1.0f, 0.5f, 0.0f, 1.0f)  // Orange
};
Cube::Cube() {
    // Default constructor (not used in this example)
}
Cube::Cube(mat4 view, mat4 projection, vec3 position) {
    viewMat = view;
    projectionMat = projection;
    SetupMatrices(view, projection, position);
    GenerateArray();
    bind();
    RandomOffset = rand() % 1000; // Generate a random offset for shader use
    //cout<<"Cube created at position: " << position.x << ", " << position.y << ", " << position.z << endl;
}
void Cube::SetupMatrices(mat4 view, mat4 projection, vec3 position) {
    modelMatrix = mat4(1.0f);
    modelMatrix = translate(modelMatrix, position);
}
void Cube::GenerateArray() {
    // Generate vertex array and buffer objects here
    
}
void Cube::bind() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Build full path to shaders (relative to executable location)
    
    // Load into member variables so they persist
    vertexShaderStr = loadShader(vertexShaderSource);
    fragmentShaderStr = loadShader(fragmentShaderSource);

    const char* vertexShaderCode = vertexShaderStr.c_str();
    const char* fragmentShaderCode = fragmentShaderStr.c_str();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderCode, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragment);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex); 
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
void Cube::render(mat4 &view, mat4 &projection, int count) {
    viewMat = view;
    projectionMat = projection;
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glUseProgram(shaderProgram);
    
    // Send matrices to shader
    //modelMatrix = rotate(modelMatrix, radians(0.5f), vec3(0.0f, 1.0f, 0.0f));

    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    int projLoc = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projectionMat[0][0]);

    int colorLoc = glGetUniformLocation(shaderProgram, "colors");
    int scrambledLoc = glGetUniformLocation(shaderProgram, "scrambled");
    int randoffsetLoc = glGetUniformLocation(shaderProgram, "randoffset");
    glUniform4fv(colorLoc, 6, &colors[0].r);
    glUniform1i(scrambledLoc, true);
    glUniform1i(randoffsetLoc, RandomOffset);

    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}
string Cube::loadShader(const char *path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open shader file: " << path << endl;
        return "";
    }
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}*/