#version 450

layout(location = 0) in vec3 inPosition;  // Position du vertex en 2D
layout(location = 1) in vec3 inColor;     // Couleur du vertex
layout(location = 2) in vec3 inNormal;    // Couleur du vertex
layout(location = 3) in vec2 inUV;        // Couleur du vertex

layout(set = 0, binding = 0) uniform CameraBuffer
{
    mat4 view;
    mat4 proj;
} camera_vp;

layout(set = 1, binding = 0) uniform ObjectBuffer
{
    mat4 model;
} object;

layout(location = 0) out vec3 fragColor;  // Couleur passée au fragment shader
layout(location = 1) out vec3 fragNormal; // Couleur passée au fragment shader
layout(location = 2) out vec2 fragUV;     // Couleur passée au fragment shader

void main() 
{
    vec4 pos = camera_vp.proj * camera_vp.view * object.model * vec4(inPosition, 1.0); // Position en 2D
    gl_Position = pos;
    fragColor = inColor; // Passe la couleur au fragment shader
}