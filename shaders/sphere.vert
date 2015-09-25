#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 texCoords;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec3 vertPositions;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 MV_light;
uniform vec4 lightPos;

out vec3 normal;
out vec3 v;
out vec3 light_dir;

void main()
{
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertPositions, 1.0);

    normal = normals;

    vec3 l = vec3(MV * lightPos);
    v = vec3(MV * vec4(vertPositions, 1.0));
    light_dir = normalize(l - v);
}

