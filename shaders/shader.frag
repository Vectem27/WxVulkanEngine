#version 450

struct BaseLightData
{
    vec3 pos;
    vec3 dir;
    float len;
    float angle;
};

struct InLightData
{
    vec4 lightSpacePos;
};

struct PixelInfo
{
    vec3 color;
    vec3 normal;
    vec2 uv;
    vec3 worldPosition;
    vec3 worldNormal;
};

layout(set = 2, binding = 0) uniform sampler2DShadow shadowMap;

layout(set = 2, binding = 2) uniform LightBuffer
{
    BaseLightData light;
} lightData;


layout(location = 0) in PixelInfo data;
layout(location = 10) in InLightData inLightPixelData;

layout(location = 0) out vec4 outColor;

float calculateShadow(vec4 lightSpacePos, float diffuse) 
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5;

    if(projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    // Évite l’auto-shadowing
    float bias = 0.00005 * exp(2.0f - diffuse);

    // Lecture de la shadow map avec comparaison auto
    return texture(shadowMap, vec3(projCoords.xy, projCoords.z - bias)) * diffuse;
}

void main() 
{
    float diffuse = max(dot(normalize(data.worldNormal), normalize(lightData.light.pos - data.worldPosition)), 0.0);
    float light = calculateShadow(inLightPixelData.lightSpacePos, diffuse);

    if (distance(vec3(inLightPixelData.lightSpacePos), lightData.light.pos) > lightData.light.len) 
        light = 0.0f;

    float angle = acos(dot(normalize(-lightData.light.dir), normalize(lightData.light.pos - data.worldPosition)));
    if ( angle > lightData.light.angle / 2)
        light = 0.0f;

    vec3 lighting = min((0.2 + light), 1) * data.color;
    outColor = vec4(lighting, 1.0);

}