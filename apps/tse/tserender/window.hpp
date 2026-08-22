#pragma once

#include <components/macros/class.hpp>

#include <SDL3/SDL_video.h>

namespace TSE
{
    class Window final
    {
    public:
        TSE_NON_COPYABLE_NON_MOVABLE(Window)

        explicit Window();
        ~Window();

        SDL_Window* getSDLWindow() const { return mWindow; }

    private:
        SDL_Window* mWindow = nullptr;
    };
}
