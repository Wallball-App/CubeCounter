#include <UI.h>
#include <Array.h>
#include <Calculation.h>
#include <gmpxx.h>


 bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

ImFont* textfont;
ImFont* titlefont;
int layercount = 3;
bool cubescrambled = false;
bool scrambledisabled;
Array* cube_array;
Calculation* calculation;

static mpz_class number;
static std::string numcombos;
static std::string numcombos_scientific;
static std::string numdigits;
static std::string numtrailzeros;

UI::UI(SDL_Window* window, SDL_GLContext &context, const char* version, Array &main_array, Calculation &calc) {
    cube_array = &main_array;
    calculation = &calc;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    this->io = &ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    textfont = io->Fonts->AddFontFromFileTTF("Fonts/Roboto-VariableFont_wdth,wght.ttf", 18.0);
    titlefont = io->Fonts->AddFontFromFileTTF("Fonts/Oswald-VariableFont_wght.ttf", 48.0);

    ImGui::StyleColorsLight();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(1.0f);
    style.FontScaleDpi = 1.0f;

    ImGui_ImplSDL3_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(version);



    number = calculation->Calculate(layercount);
    numcombos = number.get_str();
    numcombos_scientific = calculation->returnScientific(number, 4);
    numdigits = std::to_string(calculation->returnNumDigits(layercount));
    numtrailzeros = std::to_string(calculation->returnTrailingZeros(layercount));
}
void UI::render() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();

    ImGui::NewFrame();

    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.25f, 0.25f, 0.25f, 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.15f, 0.15f, 0.15f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.2f, 0.2f, 0.2f, 0.6f));


    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.1f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.15f, 0.15f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 0.6f));

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    //flags |= ImGuiWindowFlags_AlwaysAutoResize;
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    float width = 250.0f;
    float height = viewport->WorkSize.y;
    ImGui::SetNextWindowPos(viewport->WorkPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, height), ImGuiCond_Always);

    ImGui::Begin("SideBar", NULL, flags);

    ImGui::PushFont(titlefont);
    ImGui::Text("CubeCounter v1.0");
    ImGui::PopFont();

    if(ImGui::InputInt("##LayerCount", &layercount)) {
        (*cube_array).SetVars(layercount, cubescrambled);
        number = calculation->Calculate(layercount);
        numcombos = number.get_str();
        numcombos_scientific = calculation->returnScientific(number, 4);
        numdigits = std::to_string(calculation->returnNumDigits(layercount));
        numtrailzeros = std::to_string(calculation->returnTrailingZeros(layercount));
    }
    scrambledisabled = (layercount > 2048 || layercount < 2);
    if(scrambledisabled) cubescrambled = false;
    ImGui::BeginDisabled(scrambledisabled);
    if(ImGui::Checkbox("Scramble", &cubescrambled)) {
        (*cube_array).SetVars(layercount, cubescrambled);
    }
    ImGui::EndDisabled();

    UpdateCalculation(layercount);

    ImGui::End();

    ImGui::PopStyleColor(8);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void UI::quit() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
void UI::UpdateCalculation(int idx) {
    ImGui::TextWrapped("Number of Combinations: ");
    if(calculation->returnNumDigits(idx) > 20) {
        ImGui::TextWrapped(numcombos_scientific.c_str());
    } else {
        ImGui::TextWrapped(numcombos.c_str());
    }
    ImGui::TextWrapped("Number of Combinations (Scientific): ");
    ImGui::TextWrapped(numcombos_scientific.c_str());

    ImGui::TextWrapped("Number of Digits: ");
    ImGui::TextWrapped(numdigits.c_str());

    ImGui::TextWrapped("Number of Trailing Zeros: ");
    ImGui::TextWrapped(numtrailzeros.c_str());
}