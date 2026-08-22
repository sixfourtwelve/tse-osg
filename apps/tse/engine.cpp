#include "engine.hpp"

#include "tserender/graphics.hpp"
#include "tseui/debugui.hpp"

#include <components/debug/debuglog.hpp>

#include <SDL3/SDL.h>
#include <imgui.h>

#include <memory>
#include <stdexcept>
#include <string>

namespace
{
    std::runtime_error sdlError(const char* operation)
    {
        return std::runtime_error(std::string(operation) + ": " + SDL_GetError());
    }

    void setGLAttribute(SDL_GLAttr attribute, int value)
    {
        if (!SDL_GL_SetAttribute(attribute, value))
            throw sdlError("Failed to configure an OpenGL attribute");
    }
}

Engine::Engine()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        throw sdlError("Failed to initialize SDL");

    try
    {
        setGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        setGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        setGLAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        setGLAttribute(SDL_GL_DOUBLEBUFFER, 1);
        setGLAttribute(SDL_GL_DEPTH_SIZE, 24);
        setGLAttribute(SDL_GL_STENCIL_SIZE, 8);

        mWindow = SDL_CreateWindow("TSE", 1280, 720,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_MAXIMIZED);
        if (mWindow == nullptr)
            throw sdlError("Failed to create the SDL window");

        Log(Debug::Info) << "Created SDL window: " << SDL_GetWindowTitle(mWindow);

        mGLContext = SDL_GL_CreateContext(mWindow);
        if (mGLContext == nullptr)
            throw sdlError("Failed to create the OpenGL context");

        Log(Debug::Info) << "Created OpenGL context: " << SDL_GL_GetCurrentContext();

        mGraphics = std::make_unique<TSE::Graphics>(mWindow, mGLContext);
        mDebugUI = std::make_unique<TSE::DebugUI>(mWindow, mGLContext);
    }
    catch (...)
    {
        Log(Debug::Error) << "Engine initialization failed, cleaning up resources";
        mDebugUI.reset();
        mGraphics.reset();

        if (mGLContext != nullptr)
            SDL_GL_DestroyContext(mGLContext);
        if (mWindow != nullptr)
            SDL_DestroyWindow(mWindow);

        SDL_Quit();
        throw;
    }
}

Engine::~Engine()
{
    Log(Debug::Info) << "Shutting down engine and cleaning up resources";
    mDebugUI.reset();
    mGraphics.reset();

    if (mGLContext != nullptr)
        SDL_GL_DestroyContext(mGLContext);
    if (mWindow != nullptr)
        SDL_DestroyWindow(mWindow);

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
