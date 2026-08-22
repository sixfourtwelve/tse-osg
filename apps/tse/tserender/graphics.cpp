#include "graphics.hpp"

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Program>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Viewport>

#include <cmath>
#include <stdexcept>
#include <string>

namespace
{
    osg::ref_ptr<osg::Shader> loadShader(osg::Shader::Type type, const char* path)
    {
        osg::ref_ptr<osg::Shader> shader = new osg::Shader(type);

        if (!shader->loadShaderSourceFromFile(path))
            throw std::runtime_error(std::string("Could not load shader: ") + path);

        return shader;
    }

    std::runtime_error sdlError(const char* operation)
    {
        return std::runtime_error(std::string(operation) + ": " + SDL_GetError());
    }
}

namespace TSE
{
    Graphics::Graphics(SDL_Window* window, SDL_GLContext glContext)
        : mWindow(window)
        , mGLContext(glContext)
    {
        if (mWindow == nullptr || mGLContext == nullptr)
            throw std::invalid_argument("Graphics requires a valid SDL window and OpenGL context");

        if (!SDL_GL_MakeCurrent(mWindow, mGLContext))
            throw sdlError("Failed to make the OpenGL context current");

        SDL_GL_SetSwapInterval(1);

        if (!SDL_GetWindowSizeInPixels(mWindow, &mWidth, &mHeight))
            throw sdlError("Failed to query the window size");

        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        vertices->push_back(osg::Vec3(-0.5F, -0.5F, 0.0F));
        vertices->push_back(osg::Vec3(0.5F, -0.5F, 0.0F));
        vertices->push_back(osg::Vec3(0.0F, 0.5F, 0.0F));

        osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
        geometry->setVertexArray(vertices.get());
        geometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 3));

        osg::ref_ptr<osg::Program> program = new osg::Program;
        program->addShader(loadShader(osg::Shader::VERTEX, "assets/shaders/triangle.vert"));
        program->addShader(loadShader(osg::Shader::FRAGMENT, "assets/shaders/triangle.frag"));

        osg::StateSet* stateSet = geometry->getOrCreateStateSet();
        stateSet->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

        mPositionUniform = new osg::Uniform("uPosition", osg::Vec3(0.0F, 0.0F, 0.0F));
        stateSet->addUniform(mPositionUniform.get());
        stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);

        mColorUniform = new osg::Uniform("uColor", osg::Vec4(1.0F, 0.0F, 0.0F, 1.0F));
        stateSet->addUniform(mColorUniform.get());

        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable(geometry.get());

        mGraphicsWindow = new osgViewer::GraphicsWindowEmbedded(0, 0, mWidth, mHeight);

        mViewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
        mViewer.setSceneData(geode.get());

        osg::Camera* camera = mViewer.getCamera();
        camera->setGraphicsContext(mGraphicsWindow.get());
        camera->setViewMatrixAsLookAt(osg::Vec3d(0.0, 0.0, 2.0), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 1.0, 0.0));

        resize(mWidth, mHeight);
        mViewer.realize();
    }

    Graphics::~Graphics()
    {
        mViewer.setDone(true);
    }

    void Graphics::beginFrame()
    {
        if (!SDL_GL_MakeCurrent(mWindow, mGLContext))
            throw sdlError("Failed to make the OpenGL context current");

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
        mPositionUniform->set(osg::Vec3(std::sin(mTime) * 0.5F, std::cos(mTime) * 0.5F, 0.0F));
        mColorUniform->set(osg::Vec4((std::sin(mTime) + 1.0F) * 0.5F, (std::cos(mTime) + 1.0F) * 0.5F, 0.0F, 1.0F));
        mViewer.frame();
    }

    void Graphics::endFrame()
    {
        if (!SDL_GL_SwapWindow(mWindow))
            throw sdlError("Failed to swap the OpenGL window");
    }

    void Graphics::resize(int width, int height)
    {
        mWidth = width;
        mHeight = height;

        mGraphicsWindow->resized(0, 0, width, height);

        osg::Camera* camera = mViewer.getCamera();
        camera->setViewport(0, 0, width, height);

        if (width > 0 && height > 0)
        {
            camera->setProjectionMatrixAsPerspective(
                60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 1000.0);
        }
    }
}
