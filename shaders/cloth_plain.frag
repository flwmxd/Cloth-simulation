#version 330 core

uniform sampler2D Tex;
uniform sampler2D NormalMap;

uniform mat3 normalMatrix;
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform float specularity;
uniform float bumpyness;

in vec2 UV;
in vec3 normal;
in vec3 v;
in vec3 light_dir;
in vec3 Ps;
in vec3 Pt;

out vec4 color;

vec4 calcShading( vec3 N, vec3 L, mat3 MVT )
{
    //Ambient contribution
    vec4 Iamb = lightAmbient;

    //Diffuse contribution
    vec4 Idiff = lightDiffuse * max(dot(N,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    //Specular contribution
    vec3 E = -normalize(MVT * v);
    vec3 R = normalize(reflect(-L, N));
    //const float specExp = 10.0;
    vec4 Ispec = lightSpecular
        * pow(max(dot(R,E),0.0), specularity);
    Ispec = clamp(Ispec, 0.0, 1.0);

    return Iamb + Idiff + (Ispec * 0.2);
}

void main()
{
    color = texture(Tex, UV.st);

    vec4 normal_data = texture(NormalMap, UV.st);

    mat3 MVT = transpose(mat3(Ps, Pt, normalize(normal)));

    vec3 light_t = MVT * light_dir;

    vec3 normal_prime = normalize(vec3(((2.0 * normal_data.r) - 1.0)*bumpyness, 
                  ((2.0 * normal_data.g) - 1.0) * bumpyness,
                  ((2.0 * normal_data.b) - 1.0)));

    //vec3 normal_v = inverse(MVT) * normal_prime;

    //vec4 asd = texture(NormalMap, UV.st);
    //color = vec4(0.0, 0.0, 0.0, 0.0);
    // Ugly hack for the checkered floor
    //color.rgb = (color.r > 0.5) ? vec3(1.0, 1.0, 1.0) : vec3(0.0, 0.0, 0.0);
    color.rgb *= calcShading(normalMatrix * normal_prime, light_dir, MVT).rgb;
    //color
    //color.a = 1.0;
    //color.rgb = vec3(0.8, 0.4, 0.4);
}