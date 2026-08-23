#include "window.hpp"

#include <SDL3/SDL_mouse.h>
#include <components/sdlhelpers/error.hpp>

namespace TSE
{
    Window::Window()
    {
        mWindow = SDL_CreateWindow("TSE", 1280, 720,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_MAXIMIZED);
        if (mWindow == nullptr)
            throw sdlError("Failed to create the SDL window");

        if (!SDL_RaiseWindow(mWindow))
            throw sdlError("Failed to raise the SDL window");

        SDL_SetWindowRelativeMouseMode(mWindow, true);
    }

    Window::~Window()
    {
        if (mWindow != nullptr)
            SDL_DestroyWindow(mWindow);
    }
}
