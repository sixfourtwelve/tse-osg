#include "graphics.hpp"

#include <components/debug/debuglog.hpp>
#include <components/sdlhelpers/error.hpp>

#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>

#include <osg/Math>
#include <osg/StateAttribute>
#include <osg/StateSet>
#include <osg/Texture>

#include <osgDB/ReadFile>

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

namespace TSE
{
    namespace
    {
        constexpr double MouseSensitivity = 0.0025;
    }

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
        {
            throw sdlError("Failed to query the window size");
        }

        mCamera = mViewer.getCamera();

        mGraphicsWindow = new osgViewer::GraphicsWindowEmbedded(0, 0, mWidth, mHeight);

        mViewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

        mCamera->setGraphicsContext(mGraphicsWindow.get());

        resize(mWidth, mHeight);

        /*
         * Sky
         */

        mSkyboxImage = osgDB::readImageFile("assets/textures/skybox-day.png");

        if (!mSkyboxImage)
            throw std::runtime_error("Failed to load skybox image");

        mSkyboxTexture = new osg::Texture2D(mSkyboxImage.get());

        mSkyboxTexture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);

        mSkyboxTexture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

        mSkyboxSphere = new osg::Sphere(osg::Vec3(), 100.0F);

        mSkyboxDrawable = new osg::ShapeDrawable(mSkyboxSphere.get());

        mSkyboxGeode = new osg::Geode;

        mSkyboxGeode->addDrawable(mSkyboxDrawable.get());

        osg::StateSet* stateSet = mSkyboxGeode->getOrCreateStateSet();

        stateSet->setTextureAttributeAndModes(0, mSkyboxTexture.get(), osg::StateAttribute::ON);

        stateSet->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);

        mRoot = new osg::Group;

        mRoot->addChild(mSkyboxGeode.get());

        mViewer.setSceneData(mRoot.get());

        updateCamera();

        mViewer.realize();

        mFpsSampleStart = SDL_GetTicks();
    }

    Graphics::~Graphics()
    {
        mViewer.setDone(true);

        if (mGLContext != nullptr)
            SDL_GL_DestroyContext(mGLContext);
    }

    void Graphics::onEvent(const SDL_Event& event)
    {
        if (event.type != SDL_EVENT_MOUSE_MOTION)
            return;

        mYaw -= static_cast<double>(event.motion.xrel) * MouseSensitivity;

        mPitch -= static_cast<double>(event.motion.yrel) * MouseSensitivity;

        mPitch = std::clamp(mPitch, -osg::PI_2 + 0.01, osg::PI_2 - 0.01);
    }

    void Graphics::beginFrame()
    {
        int width = 0;
        int height = 0;

        if (!SDL_GetWindowSizeInPixels(mWindow, &width, &height))
        {
            throw sdlError("Failed to query the window size");
        }

        if (width != mWidth || height != mHeight)
        {
            resize(width, height);
        }

        updateCamera();
    }

    void Graphics::draw()
    {
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
        {
            throw sdlError("Failed to update the window title");
        }

        mFpsSampleStart = now;
        mFramesInSample = 0;
    }

    void Graphics::resize(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        mGraphicsWindow->resized(0, 0, width, height);

        mCamera->setViewport(0, 0, width, height);

        if (width <= 0 || height <= 0)
        {
            return;
        }

        mCamera->setProjectionMatrixAsPerspective(
            60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 1000.0);
    }

    void Graphics::updateCamera()
    {
        const osg::Vec3d direction{ std::sin(mYaw) * std::cos(mPitch), -std::cos(mYaw) * std::cos(mPitch),
            std::sin(mPitch) };

        mCamera->setViewMatrixAsLookAt(mCameraPosition, mCameraPosition + direction, osg::Vec3d(0.0, 0.0, 1.0));
    }
}