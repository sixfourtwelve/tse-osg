#pragma once

#include <components/macros/class.hpp>

#include <SDL3/SDL_video.h>
#include <memory>

namespace TSE
{
    class DebugUI;
    class Graphics;
    class Window;
}

class Engine final
{
public:
    TSE_NON_COPYABLE_NON_MOVABLE(Engine)

    Engine();
    ~Engine();

    void go();

private:
    SDL_GLContext mGLContext = nullptr;
    std::unique_ptr<TSE::Window> mWindow;
    std::unique_ptr<TSE::Graphics> mGraphics;
    std::unique_ptr<TSE::DebugUI> mDebugUI;
};
