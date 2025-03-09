#version 450
layout(location = 0) in vec3 inPosition; // Position du vertex en 2D
layout(location = 1) in vec3 inColor;    // Couleur du vertex

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) out vec3 fragColor; // Couleur passée au fragment shader

void main() 
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0); // Position en 2D
    fragColor = inColor; // Passe la couleur au fragment shader
}