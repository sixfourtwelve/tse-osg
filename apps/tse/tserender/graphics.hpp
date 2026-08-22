#pragma once

#include <SDL3/SDL_video.h>
#include <components/macros/class.hpp>
#include <osg/Uniform>
#include <osg/ref_ptr>
#include <osgViewer/Viewer>

namespace TSE
{
    class Graphics final
    {
    public:
        TSE_NON_COPYABLE_NON_MOVABLE(Graphics)

        Graphics(SDL_Window* window);
        ~Graphics();

        void beginFrame();
        void draw();
        void endFrame();

        SDL_GLContext getGLContext() const { return mGLContext; }

    private:
        void resize(int width, int height);

        Uint64 mFpsSampleStart = 0;
        Uint64 mFramesInSample = 0;
        int mWidth = 0;
        int mHeight = 0;
        float mTime = 0.0F;

        SDL_Window* mWindow;
        SDL_GLContext mGLContext;
        osg::ref_ptr<osg::Camera> mCamera;
        osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
        osgViewer::Viewer mViewer;
    };
}
