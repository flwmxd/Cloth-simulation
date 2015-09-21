#version 330 core

//uniform sampler2D Tex;

out vec4 color;
uniform vec4 in_color;

void main()
{
    color = in_color;
}