#include "graphics.hpp"

#include <components/debug/debuglog.hpp>
#include <components/sdlhelpers/error.hpp>

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/Program>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Transform>
#include <osg/Viewport>
#include <osgDB/ReadFile>

#include <stdexcept>

namespace TSE
{
    Graphics::Graphics(SDL_Window* window)
        : mWindow(window)
    {
        if (mWindow == nullptr)
            throw std::invalid_argument("Graphics requires a valid SDL window");

        mGLContext = SDL_GL_CreateContext(mWindow);
        if (mGLContext == nullptr)
            throw sdlError("Failed to create the OpenGL context");

        if (!SDL_GL_MakeCurrent(mWindow, mGLContext))
            throw sdlError("Failed to make the OpenGL context current");

        SDL_GL_SetSwapInterval(1);
        SDL_SetWindowTitle(mWindow, "TSE - FPS: measuring...");

        Log(Debug::Info) << "Created OpenGL context: " << SDL_GL_GetCurrentContext();

        if (!SDL_GetWindowSizeInPixels(mWindow, &mWidth, &mHeight))
            throw sdlError("Failed to query the window size");

        mCamera = mViewer.getCamera();

        mGraphicsWindow = new osgViewer::GraphicsWindowEmbedded(0, 0, mWidth, mHeight);

        mViewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

        osg::Camera* camera = mViewer.getCamera();
        camera->setGraphicsContext(mGraphicsWindow.get());
        camera->setViewMatrixAsLookAt(osg::Vec3d(0.0, 0.0, 2.0), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 1.0, 0.0));

        resize(mWidth, mHeight);
        mViewer.realize();
        mFpsSampleStart = SDL_GetTicks();
    }

    Graphics::~Graphics()
    {
        if (mGLContext != nullptr)
            SDL_GL_DestroyContext(mGLContext);
        mViewer.setDone(true);
    }

    void Graphics::beginFrame()
    {
        int width = 0;
        int height = 0;
        if (!SDL_GetWindowSizeInPixels(mWindow, &width, &height))
            throw sdlError("Failed to query the window size");

        if (width != mWidth || height != mHeight)
            resize(width, height);
    }

    void Graphics::draw()
    {
        mTime += 0.01F;
        mViewer.frame();
    }

    void Graphics::endFrame()
    {
        if (!SDL_GL_SwapWindow(mWindow))
            throw sdlError("Failed to swap the OpenGL window");

        ++mFramesInSample;

        const Uint64 now = SDL_GetTicks();
        const Uint64 elapsedMs = now - mFpsSampleStart;

        if (elapsedMs < 1000)
            return;

        const double fps = static_cast<double>(mFramesInSample) * 1000.0 / static_cast<double>(elapsedMs);

        const std::string title = "TSE - FPS: " + std::to_string(static_cast<unsigned int>(fps));

        if (!SDL_SetWindowTitle(mWindow, title.c_str()))
            throw sdlError("Failed to update the window title");

        mFpsSampleStart = now;
        mFramesInSample = 0;
    }

    void Graphics::resize(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        mGraphicsWindow->resized(0, 0, width, height);

        mCamera->setViewport(0, 0, width, height);

        if (width > 0 && height > 0)
        {
            mCamera->setProjectionMatrixAsPerspective(
                60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 1000.0);
        }
    }
}
