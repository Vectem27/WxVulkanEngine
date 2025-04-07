#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in vec4 fragPosLightSpace;

layout(set = 2, binding = 0) uniform sampler2DShadow shadowMap;

layout(location = 0) out vec4 outColor;

float calculateShadow(vec4 lightSpacePos) 
{
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords.xy = projCoords.xy * 0.5 + 0.5;

    if(projCoords.z > 1.0 || projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
        return 0.0;

    // Évite l’auto-shadowing
    float bias = 0.00005;

    // Lecture de la shadow map avec comparaison auto
    return texture(shadowMap, vec3(projCoords.xy, projCoords.z - bias));
}

void main() {
    float shadow = calculateShadow(fragPosLightSpace);
    vec3 lighting = min((0.2 + vec3(shadow)), 1) * fragColor;
    outColor = vec4(lighting, 1.0); //vec4(lighting, 1.0);
    //outColor = vec4(vec3((fragPosLightSpace.z / fragPosLightSpace.w)), 1.0);
}