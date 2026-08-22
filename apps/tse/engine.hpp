#pragma once

#include <components/macros/class.hpp>

#include <SDL3/SDL_video.h>
#include <memory>

namespace TSE
{
    class DebugUI;
    class Graphics;
}

class Engine final
{
public:
    Engine();
    ~Engine();

    TSE_NON_COPYABLE_NON_MOVABLE(Engine)

    void go();

private:
    SDL_Window* mWindow = nullptr;
    SDL_GLContext mGLContext = nullptr;
    std::unique_ptr<TSE::Graphics> mGraphics;
    std::unique_ptr<TSE::DebugUI> mDebugUI;
};
