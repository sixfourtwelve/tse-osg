#pragma once

#include <components/macros/class.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

namespace TSE
{
    class DebugUI final
    {
    public:
        DebugUI(SDL_Window* window, SDL_GLContext glContext);
        ~DebugUI();

        TSE_NON_COPYABLE_NON_MOVABLE(DebugUI)

        void onEvent(const SDL_Event& event);
        void beginFrame();
        void endFrame();
    };
}
