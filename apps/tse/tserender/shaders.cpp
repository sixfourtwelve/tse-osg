#include "shaders.hpp"

namespace TSE
{
    osg::ref_ptr<osg::Shader> loadShader(osg::Shader::Type type, const char* path)
    {
        osg::ref_ptr<osg::Shader> shader = new osg::Shader(type);

        if (!shader->loadShaderSourceFromFile(path))
            throw std::runtime_error(std::string("Could not load shader: ") + path);

        return shader;
    }
}
