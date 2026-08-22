#include <SDL3/SDL.h>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Program>
#include <osg/Shader>
#include <osg/StateSet>
#include <osg/Uniform>
#include <osg/Viewport>

#include <osgViewer/GraphicsWindow>
#include <osgViewer/Viewer>

#include <iostream>

static osg::ref_ptr<osg::Shader> loadShader(osg::Shader::Type type, const char* path)
{
    osg::ref_ptr<osg::Shader> shader = new osg::Shader(type);

    if (!shader->loadShaderSourceFromFile(path))
    {
        std::cerr << "Could not load shader: " << path << '\n';
        return nullptr;
    }

    return shader;
}

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_Window* window = SDL_CreateWindow("TSE", 1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_MAXIMIZED);

    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';

        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!context)
    {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << '\n';

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1);

    int width = 0;
    int height = 0;

    SDL_GetWindowSizeInPixels(window, &width, &height);

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

    vertices->push_back(osg::Vec3(-0.5f, -0.5f, 0.0f));
    vertices->push_back(osg::Vec3(0.5f, -0.5f, 0.0f));
    vertices->push_back(osg::Vec3(0.0f, 0.5f, 0.0f));

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
    geometry->setVertexArray(vertices.get());

    geometry->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, 3));

    osg::ref_ptr<osg::Shader> vertexShader = loadShader(osg::Shader::VERTEX, "shaders/triangle.vert");
    osg::ref_ptr<osg::Shader> fragmentShader = loadShader(osg::Shader::FRAGMENT, "shaders/triangle.frag");

    if (!vertexShader || !fragmentShader)
        return 1;

    osg::ref_ptr<osg::Program> program = new osg::Program;

    program->addShader(vertexShader.get());
    program->addShader(fragmentShader.get());

    osg::StateSet* stateSet = geometry->getOrCreateStateSet();

    stateSet->setAttributeAndModes(program.get(), osg::StateAttribute::ON);

    osg::ref_ptr<osg::Uniform> positionUniform = new osg::Uniform("uPosition", osg::Vec3(0.0f, 0.0f, 0.0f));

    stateSet->addUniform(positionUniform.get());

    osg::ref_ptr<osg::Group> root = new osg::Group;

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    root->addChild(geode.get());

    geode->addDrawable(geometry.get());

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphicsWindow
        = new osgViewer::GraphicsWindowEmbedded(0, 0, width, height);

    osgViewer::Viewer viewer;

    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

    viewer.setSceneData(geode.get());

    osg::Camera* camera = viewer.getCamera();

    camera->setGraphicsContext(graphicsWindow.get());

    camera->setViewport(new osg::Viewport(0, 0, width, height));

    camera->setViewMatrixAsLookAt(osg::Vec3d(0.0, 0.0, 2.0), osg::Vec3d(0.0, 0.0, 0.0), osg::Vec3d(0.0, 1.0, 0.0));

    camera->setProjectionMatrixAsPerspective(
        60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 1000.0);

    viewer.realize();

    bool running = true;

    float time = 0.0f;
    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)
                running = false;

            if (event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
            {
                SDL_GetWindowSizeInPixels(window, &width, &height);

                graphicsWindow->resized(0, 0, width, height);
                camera->setViewport(0, 0, width, height);

                if (height > 0)
                    camera->setProjectionMatrixAsPerspective(
                        60.0, static_cast<double>(width) / static_cast<double>(height), 0.1, 1000.0);
            }
        }

        time += 0.01f;

        positionUniform->set(osg::Vec3(sinf(time) * 0.5f, cosf(time) * 0.5f, 0.0f));

        SDL_GL_MakeCurrent(window, context);

        viewer.frame();

        SDL_GL_SwapWindow(window);
    }

    viewer.setDone(true);

    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
