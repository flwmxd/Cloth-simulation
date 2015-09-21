#version 330 core

// Input vertex data, different for all executions of this shader.
//layout(location = 1) in vec3 normals;
layout(location = 0) in vec3 vertPositions;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 bitangents;

uniform mat4 MVP;
uniform mat4 MV;
uniform mat4 MV_light;
uniform vec4 lightPos;

out vec2 UV;
out vec3 normal;
out vec3 v;
out vec3 light_dir;
out vec3 Ps;
out vec3 Pt;

void main()
{
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(vertPositions, 1.0);
    UV = texCoords;
    v = vec3(MV * vec4(vertPositions, 1.0));
    normal = normals;
    //vec4 lpos = vec4(0.5, 10.0, -10.0, 1.0);
    vec3 l = vec3(MV * lightPos);
    light_dir = normalize(l - v);

    Ps = normalize( mat3(MV) * tangents );
    Pt = normalize( mat3(MV) * bitangents );
    //light_dir = normalize(vec3(1.0, 1.0, 1.0));
}

