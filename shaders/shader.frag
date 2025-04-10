#version 450

struct BaseLightData
{
    mat4 viewProj;
    vec3 pos;
    vec3 dir;
    float len;
    float angle;
};

struct PixelInfo
{
    vec3 color;
    vec3 normal;
    vec2 uv;
    vec3 worldPosition;
    vec3 worldNormal;
};

layout(set = 2, binding = 0) uniform sampler2DShadow shadowMaps[16];

layout(set = 2, binding = 1) uniform LightBuffer
{
    int num;
    BaseLightData light[16];
} lightData;


layout(location = 0) in PixelInfo data;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outPos;

float calculateShadow(vec4 lightSpacePos, float diffuse, sampler2DShadow shadowMap) 
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
    float light = 0.0;
    int numSpotlightLights = lightData.num;
    for (int i = 0; i < 1; i++)
    {
        vec4 lightSpacePos = lightData.light[i].viewProj * vec4(data.worldPosition, 1.0f);
        float diffuse = max(dot(normalize(data.worldNormal), normalize(lightData.light[i].pos - data.worldPosition)), 0.0);
        float currentLight = calculateShadow(lightSpacePos, diffuse, shadowMaps[i]);

        if (distance(data.worldPosition, lightData.light[i].pos) > lightData.light[i].len) 
            currentLight = 0.0f;

        float angle = acos(dot(normalize(-lightData.light[i].dir), normalize(lightData.light[i].pos - data.worldPosition)));
        if (angle > lightData.light[i].angle / 2)
            currentLight = 0.0f;

        light += currentLight;
    }



    vec3 lighting = min((0.2 + light), 1.0) * data.color;
    outColor = vec4(lighting, 1.0);
    outNormal = vec4(data.worldNormal * 0.5 + 0.5, 1.0f);
    outPos = vec4(data.worldPosition, 1.0f);
}
