#include "debugui.hpp"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <stdexcept>

namespace TSE
{
    DebugUI::DebugUI(SDL_Window* window, SDL_GLContext glContext)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        if (!ImGui_ImplSDL3_InitForOpenGL(window, glContext))
        {
            ImGui::DestroyContext();
            throw std::runtime_error("Failed to initialize the ImGui SDL3 backend");
        }

        if (!ImGui_ImplOpenGL3_Init())
        {
            ImGui_ImplSDL3_Shutdown();
            ImGui::DestroyContext();
            throw std::runtime_error("Failed to initialize the ImGui OpenGL3 backend");
        }
    }

    DebugUI::~DebugUI()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void DebugUI::onEvent(const SDL_Event& event)
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
    }

    void DebugUI::beginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void DebugUI::endFrame()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
