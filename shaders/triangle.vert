#version 410 core

layout(location = 0) in vec4 osg_Vertex;

uniform mat4 osg_ModelViewProjectionMatrix;
uniform vec3 uPosition;

void main()
{
    vec4 position = osg_Vertex;
    position.xyz += uPosition;

    gl_Position =
        osg_ModelViewProjectionMatrix *
        position;
}
