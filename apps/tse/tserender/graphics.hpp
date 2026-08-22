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

    private:
        void resize(int width, int height);

        SDL_Window* mWindow;
        SDL_GLContext mGLContext;
        int mWidth = 0;
        int mHeight = 0;
        float mTime = 0.0F;
        osg::ref_ptr<osg::Camera> mCamera;
        osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;
        osgViewer::Viewer mViewer;
    };
}
