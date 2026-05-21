#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include <Array.h>
#include <Calculation.h>

#include <SDL3/SDL.h>

class UI {
    public:
        ImGuiIO* io;
        UI();
        UI(SDL_Window* window, SDL_GLContext &context, const char* version, Array &main_array, Calculation &calc);
        void render();
        void quit();
        void UpdateCalculation(int idx);
};