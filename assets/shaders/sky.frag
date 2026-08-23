// assets/shaders/sky.frag
#version 410 core

uniform sampler2D uSky;

in vec2 vUV;

out vec4 fragColor;

void main()
{
    fragColor = texture(uSky, vUV);
}