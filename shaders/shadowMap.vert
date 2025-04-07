#version 450

layout(location = 0) in vec3 inPosition;  // Position du vertex en 2D

layout(set = 0, binding = 0) uniform CameraBuffer
{
    mat4 view;
    mat4 proj;
} camera_vp;

layout(set = 1, binding = 0) uniform ObjectBuffer
{
    mat4 model;
} object;

void main() 
{
    vec4 pos = camera_vp.proj * camera_vp.view * object.model * vec4(inPosition, 1.0); // Position en 2D
    gl_Position = pos;
}