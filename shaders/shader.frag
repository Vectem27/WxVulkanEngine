#version 450

struct PixelInfo
{
    vec3 color;
    vec3 normal;
    vec2 uv;
    vec3 worldPosition;
    vec3 worldNormal;
};

layout(location = 0) in PixelInfo data;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outPos;


void main() 
{
    outColor = vec4(data.color, 1.0);
    outNormal = vec4(data.worldNormal * 0.5 + 0.5, 1.0f);
    outPos = vec4(data.worldPosition, 1.0f);
}
