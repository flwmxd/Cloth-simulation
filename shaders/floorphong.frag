#version 330 core

uniform sampler2D Tex;
uniform mat3 normalMatrix;
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
//uniform mat3 normalMatrix;

in vec2 UV;
in vec3 normal;
in vec3 v;
in vec3 light_dir;

out vec4 color;

vec3 lightDir = vec3(1.0, 1.0, 1.0);

vec4 calcShading( vec3 N, vec3 L )
{
    //Ambient contribution
    vec4 Iamb = lightAmbient;

    //Diffuse contribution
    vec4 Idiff = lightDiffuse * max(dot(N,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    return Iamb + Idiff;
}

vec4 calcSpecularShading( vec3 N, vec3 L ) {

    //Specular contribution
    vec3 v_dir = normalize(-v);
    vec3 R = normalize(reflect(-L, normalize(N)));
    const float specExp = 10.0;
    float spec = dot(R, v_dir);
    spec = (spec > 0.0) ? (1.0 * pow(spec, specExp)) : 0.0;

    vec4 Ispec = lightSpecular
        * spec;
    Ispec = clamp(Ispec, 0.0, 1.0);

    return Ispec;
}

void main()
{
    color = texture(Tex, UV.st);
    
    // Ugly hack for the checkered floor
    color.rgb = (color.r > 0.5) ? vec3(1.0, 1.0, 1.0) : vec3(0.0, 0.0, 0.0);
    color.rgb *= calcShading(normalize(normalMatrix * normal), light_dir).rgb * 1.0;
    color.rgb += calcSpecularShading(normalize(normalMatrix * normal), light_dir).rgb;
}
