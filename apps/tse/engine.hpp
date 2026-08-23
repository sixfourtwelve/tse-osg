#pragma once

#include <components/macros/class.hpp>

#include <SDL3/SDL_video.h>
#include <memory>

namespace TSE
{
    class DebugUI;
    class Graphics;
    class Window;
    class World;
}

class Engine final
{
public:
    TSE_NON_COPYABLE_NON_MOVABLE(Engine)

    explicit Engine();
    ~Engine();

    void go();

private:
    std::unique_ptr<TSE::Window> mWindow;
    std::unique_ptr<TSE::Graphics> mGraphics;
    std::unique_ptr<TSE::DebugUI> mDebugUI;
};
