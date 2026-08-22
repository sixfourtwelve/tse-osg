#include "engine.hpp"

#include "tserender/graphics.hpp"
#include "tserender/window.hpp"
#include "tseui/debugui.hpp"

#include <components/debug/debuglog.hpp>
#include <components/sdlhelpers/attributes.hpp>
#include <components/sdlhelpers/error.hpp>

#include <SDL3/SDL.h>
#include <imgui.h>

#include <memory>

Engine::Engine()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw sdlError("Failed to initialize SDL");

    try
    {
        setRequiredSDLAttributes();
        mWindow = std::make_unique<TSE::Window>();

        Log(Debug::Info) << "Created SDL window: " << SDL_GetWindowTitle(mWindow->getSDLWindow());

        mGraphics = std::make_unique<TSE::Graphics>(mWindow->getSDLWindow());
        mDebugUI = std::make_unique<TSE::DebugUI>(mWindow->getSDLWindow(), mGLContext);
    }
    catch (...)
    {
        Log(Debug::Error) << "Engine initialization failed, cleaning up resources";
        mDebugUI.reset();
        mGraphics.reset();

        if (mGLContext != nullptr)
            SDL_GL_DestroyContext(mGLContext);

        SDL_Quit();
        throw;
    }
}

Engine::~Engine()
{
    Log(Debug::Info) << "Shutting down engine and cleaning up resources";

    mDebugUI.reset();
    mGraphics.reset();
    mWindow.reset();

    SDL_Quit();
}

void Engine::go()
{
    bool running = true;

    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            mDebugUI->onEvent(event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
                running = false;
        }

        mGraphics->beginFrame();
        mDebugUI->beginFrame();

        mGraphics->draw();
        ImGui::ShowDemoWindow();

        mDebugUI->endFrame();
        mGraphics->endFrame();
    }
}
