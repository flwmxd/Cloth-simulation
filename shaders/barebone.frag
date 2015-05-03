#version 330 core

uniform sampler2D Tex;

in vec2 UV;
out vec4 color;

void main()
{
    color = texture(Tex, UV.st);
    
    // Ugly hack for the checkered floor
    color.rgb = (color.r > 0.5) ? vec3(1.0, 1.0, 1.0) : vec3(0.0, 0.0, 0.0);
}