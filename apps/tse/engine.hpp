#pragma once

#include <box3d/box3d.h>
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

    Engine();
    ~Engine();

    void go();

private:
    std::unique_ptr<TSE::Window> mWindow;
    std::unique_ptr<TSE::Graphics> mGraphics;
    std::unique_ptr<TSE::DebugUI> mDebugUI;
    std::unique_ptr<TSE::World> mPhysicsWorld;

    b3BodyDef mGroundBodyDef;
    b3BodyId mGroundBodyId;
    b3BoxHull mGroundBox;
    b3ShapeDef mGroundShapeDef;

    b3BodyDef mDynamicBodyDef;
    b3BodyId mDynamicBodyId;
    b3BoxHull mDynamicBox;
    b3ShapeDef mDynamicShapeDef;
};
