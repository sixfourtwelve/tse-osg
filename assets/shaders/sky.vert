// assets/shaders/sky.vert
#version 410 core

in vec4 osg_Vertex;
in vec2 osg_MultiTexCoord0;

uniform mat4 osg_ModelViewProjectionMatrix;

out vec2 vUV;

void main()
{
    vUV = osg_MultiTexCoord0;

    gl_Position =
        osg_ModelViewProjectionMatrix *
        osg_Vertex;
}