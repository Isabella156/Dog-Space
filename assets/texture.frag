// reference: learn opengl multiple lights, textures, JoeyDeVries
#version 430
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emissive;
}; 

struct PointLight {
    vec3 position;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define POINT_LIGHT_NUMBER 3

in vec3 v2fNormal;
in vec3 v2fPos;
in vec3 v2fColor;
in vec2 v2fTexCoord;

out vec4 oColor;

uniform Material material;
uniform PointLight pointLights[POINT_LIGHT_NUMBER];
uniform sampler2D iTexture;
layout(location = 7) uniform vec3 viewPos;

vec3 pointLightEffect(PointLight light, vec3 v2fNormal, vec3 v2fPos, vec3 viewDir);



void main(){
    vec3 viewDir = normalize(viewPos - v2fPos);
    vec3 norm = normalize(v2fNormal);
    vec3 result = vec3(0.f, 0.f, 0.f);
    // point light
    for(int i = 0; i < POINT_LIGHT_NUMBER; i ++){
        result += pointLightEffect(pointLights[i], norm, v2fPos, viewDir);
    }
    result += material.emissive;

    oColor = texture(iTexture, v2fTexCoord) * vec4(result, 1.0);
}


vec3 pointLightEffect(PointLight light, vec3 norm, vec3 v2fPos, vec3 viewDir){
    // ambient
    vec3 ambient = material.ambient * light.ambient * light.color;

    // diffuse
    vec3 lightDir = normalize(light.position - v2fPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // specular
    vec3 halfDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfDir), 0.0), 64);
    vec3 specular =  (material.specular * spec) * light.specular;

    vec3 result = (ambient + diffuse + specular) * v2fColor;
    return result;
}
