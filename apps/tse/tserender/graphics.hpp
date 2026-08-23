#pragma once

#include <components/macros/class.hpp>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

#include <osg/Camera>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Image>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Vec3d>
#include <osg/ref_ptr>

#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>

namespace TSE
{
    class Graphics final
    {
    public:
        TSE_NON_COPYABLE_NON_MOVABLE(Graphics)

        explicit Graphics(SDL_Window* window);
        ~Graphics();

        void onEvent(const SDL_Event& event);

        void beginFrame();
        void draw();
        void endFrame();

        SDL_GLContext getGLContext() const { return mGLContext; }

    private:
        void resize(int width, int height);
        void updateCamera();

        Uint64 mFpsSampleStart = 0;
        Uint64 mFramesInSample = 0;

        int mWidth = 0;
        int mHeight = 0;

        SDL_Window* mWindow = nullptr;
        SDL_GLContext mGLContext = nullptr;

        osg::ref_ptr<osg::Camera> mCamera;
        osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> mGraphicsWindow;

        osg::ref_ptr<osg::Image> mSkyboxImage;
        osg::ref_ptr<osg::Texture2D> mSkyboxTexture;
        osg::ref_ptr<osg::Sphere> mSkyboxSphere;
        osg::ref_ptr<osg::ShapeDrawable> mSkyboxDrawable;
        osg::ref_ptr<osg::Geode> mSkyboxGeode;

        osg::ref_ptr<osg::Group> mRoot;

        osg::Vec3d mCameraPosition{ 0.0, 0.0, 2.0 };

        double mYaw = 0.0;
        double mPitch = 0.0;

        osgViewer::Viewer mViewer;
    };
}