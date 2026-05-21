#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gmpxx.h>

#include <iostream>
#include <cstdio>

#include <Cube.h>
#include <Array.h>
#include <UI.h>
#include <Calculation.h>

using namespace std;


mat4 viewMatrix;
mat4 projectionMatrix;
vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
vec3 resetPos = vec3(0.0f, 0.0f, 3.0f);
vec3 forwardVector;
vec3 rightVector;
vec3 up = vec3(0.0f, 1.0f, 0.0f);

Array cubearray;
Calculation calc;

float yaw = -90.0f;
float pitch = 0.0f;

int cubesize = 3;

void PollKeystrokes(SDL_Window* window, SDL_Event &event) {
    const bool* keyboardState = SDL_GetKeyboardState(NULL);
    unsigned int mouseState = SDL_GetMouseState(NULL, NULL);
    if(keyboardState[SDL_SCANCODE_ESCAPE]) {
        SDL_Event quitEvent;
        quitEvent.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&quitEvent);
    }
    
    if(keyboardState[SDL_SCANCODE_W]) {
        cameraPos += forwardVector * cubearray.speed;
    }
    if(keyboardState[SDL_SCANCODE_S]) {
        cameraPos -= forwardVector * cubearray.speed;
    }
    if(keyboardState[SDL_SCANCODE_A]) {
        cameraPos -= rightVector * cubearray.speed;
    }
    if(keyboardState[SDL_SCANCODE_D]) {
        cameraPos += rightVector * cubearray.speed;
    }

    if(keyboardState[SDL_SCANCODE_SPACE]) {
        cameraPos += up * cubearray.speed;
    }
    if(keyboardState[SDL_SCANCODE_LSHIFT]) {
        cameraPos -= up * cubearray.speed;
    }

    if(keyboardState[SDL_SCANCODE_R]) {
        cameraPos = resetPos;
        pitch = 0.0f;
        yaw = -90.0f;
    } 
}
void RebuildMatrix(mat4 &view) {
    vec3 position = cameraPos;
    vec3 direction = vec3(cos(radians(pitch)) * cos(radians(yaw)), sin(radians(pitch)), sin(radians(yaw)) * cos(radians(pitch)));
    forwardVector = normalize(direction);
    rightVector = normalize(cross(forwardVector, vec3(0.0f, 1.0f, 0.0f)));
    up = normalize(cross(rightVector, forwardVector));
    view = lookAt(position, position + forwardVector, up);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);  
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow("CubeCounter v1.0", 1152, 648, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    // Capture mouse for motion input
    SDL_SetWindowMouseGrab(window, true);
    SDL_HideCursor();
    SDL_SetWindowRelativeMouseMode(window, true);

    if(window == nullptr) {
        cout << "Failed to create window: " << SDL_GetError() << endl;
        return -1;
    }

    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    
    resetPos = vec3(0.0f, 0.0f, 3.0f + cubesize * 1.5f);
    cameraPos = resetPos;

    viewMatrix = lookAt(resetPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projectionMatrix = perspective(radians(cubearray.FOV), 1152.0f / 648.0f, 0.1f, cubesize * 10.0f);

    calc = Calculation();
    cubearray = Array(cubesize, viewMatrix, projectionMatrix, false);
    UI ui = UI(window, context, const_cast<const char*>("#version 330 core"), cubearray, calc);
    
    bool RUNNING = true;
    SDL_Event event;

    while(RUNNING) {
        while(SDL_PollEvent(&event)) {
            
            ImGui_ImplSDL3_ProcessEvent(&event);
            if(ui.io->WantCaptureMouse || ui.io->WantCaptureKeyboard) continue;

            if(event.type == SDL_EVENT_QUIT) RUNNING = false;
            if(event.type == SDL_EVENT_MOUSE_MOTION && SDL_GetWindowMouseGrab(window)) {
                float sensitivity = 0.1f;
                yaw += event.motion.xrel * sensitivity;
                pitch -= event.motion.yrel * sensitivity;
                pitch = glm::clamp(pitch, -89.0f, 89.0f); // Limit pitch to avoid gimbal lock
            }
            if(event.type == SDL_EVENT_MOUSE_WHEEL) {
                if(event.wheel.y > 0) {
                    if(cubearray.FOV < 180.0f) cubearray.FOV -= 0.5f;
                }
                else if(event.wheel.y < 0) {
                    if(cubearray.FOV > 5.0f) cubearray.FOV += 0.5f;
                }
                projectionMatrix = perspective(radians(cubearray.FOV), 1152.0f / 648.0f, 0.1f, cubearray.cubesize * 10.0f);
            }
            if(event.type == SDL_EVENT_KEY_DOWN) {
                if(event.key.key == SDLK_LCTRL) {
                    if(SDL_GetWindowMouseGrab(window) != true) {
                        SDL_SetWindowMouseGrab(window, true);
                        SDL_HideCursor();
                        SDL_SetWindowRelativeMouseMode(window, true);
                    }
                    else {
                        SDL_ShowCursor();
                        SDL_SetWindowMouseGrab(window, false);
                        SDL_SetWindowRelativeMouseMode(window, false);
                    }
                }
            }
        }
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        PollKeystrokes(window, event);
        RebuildMatrix(viewMatrix);

        cubearray.render(viewMatrix, projectionMatrix);
        
        ui.render();

        SDL_GL_SwapWindow(window);
    }
    ui.quit();
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
