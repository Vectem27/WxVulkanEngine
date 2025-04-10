#version 450

struct BaseLightData
{
    mat4 viewProj;
    vec3 pos;
    vec3 dir;
    float len;
    float angle;
    vec3 color;
    float intensity;
    float penumbraAngle; // Angle de transition douce
    uint shadowMapIndex;
};

// G-Buffer inputs
layout(set = 0, binding = 0) uniform sampler2D gPosition;
layout(set = 0, binding = 1) uniform sampler2D gNormal;

// Lighting data
layout(set = 1, binding = 0) uniform LightUBO 
{
    BaseLightData light;
} lightBuffer;

// Shadow map
layout(set = 1, binding = 1) uniform sampler2DShadow shadowMap[16];

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

float calculateAttenuation(float distance, float range) {
    float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);
    return smoothstep(range, range * 0.8, distance) * attenuation;
}

float calculateSpotEffect(vec3 lightDir, vec3 lightToFrag, float spotAngle, float penumbraAngle) 
{
    float cosTheta = dot(normalize(-lightDir), normalize(lightToFrag));
    float cosInner = cos(spotAngle * 0.5);
    float cosOuter = cos((spotAngle + penumbraAngle) * 0.5);
    
    return smoothstep(cosOuter, cosInner, cosTheta);
}

float calculateShadow(vec4 lightSpacePos, float NoL, sampler2DShadow shadowMap) 
{   
    if (NoL <= 0.0001)
        return 0.0f;

    // Perspective divide
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    
    // Convert to [0,1] range
    projCoords.xy = projCoords.xy * 0.5 + 0.5;
    
    // Check if fragment is outside light frustum
    if (projCoords.z > 1.0 || any(lessThan(projCoords.xy, vec2(0.0))) || any(greaterThan(projCoords.xy, vec2(1.0))))
        return 0.0;

    // Calculate bias (based on surface slope)
    float bias = max(0.00025 * (1.0 - NoL), 0.00001);
    
    // Sample shadow map with PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) 
    {
        for(int y = -1; y <= 1; ++y) 
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, projCoords.z - bias));
            shadow += pcfDepth;
        }
    }
    shadow /= 9.0;
    
    return shadow * NoL;
}

void main() 
{
    // Sample G-Buffer
    vec3 worldPosition = texture(gPosition, inUV).rgb;
    vec3 worldNormal = normalize(texture(gNormal, inUV).rgb);
    
    // Get first light
    BaseLightData light = lightBuffer.light;
    
    // Light vector
    vec3 L = light.pos - worldPosition;
    float dist = length(L);
    L = normalize(L);

    // Diffuse lighting
    float NoL = max(dot(worldNormal, L), 0.0);
    
    // Initialize lighting
    vec3 lighting = vec3(0.0);
    
    // Check distance
    if (dist <= light.len)
    {
        // Check spotlight angle
        float spotEffect = calculateSpotEffect(light.dir, L, light.angle, light.penumbraAngle);
        
        if (spotEffect > 0.0) 
        {
            // Calculate attenuation
            float attenuation = calculateAttenuation(dist, light.len);
            
            // Calculate shadow
            vec4 lightSpacePos = light.viewProj * vec4(worldPosition, 1.0);
            float shadow = calculateShadow(lightSpacePos, NoL, shadowMap[light.shadowMapIndex]);
            
            // Combine all factors
            lighting = light.color * NoL * shadow * light.intensity * spotEffect;
        }
    }
    
    // Ambient term
    vec3 ambient = vec3(0.1);
    
    // Final color
    vec3 color = ambient + lighting;
    outColor = vec4(color, 1.0);
}