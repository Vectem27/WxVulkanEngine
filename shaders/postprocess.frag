#version 450

layout(set = 0, binding = 0) uniform sampler2D gColor;
layout(set = 0, binding = 1) uniform sampler2D lighting;

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

void main() 
{
    vec3 color = texture(gColor, inUV).rgb;
    vec3 light = texture(lighting, inUV).rgb;
    
    outColor = vec4(color * light, 1.0f);
}