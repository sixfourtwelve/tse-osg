#pragma once

#include <osg/Shader>
#include <osg/ref_ptr>

namespace TSE
{
    osg::ref_ptr<osg::Shader> loadShader(osg::Shader::Type type, const char* path);
}
