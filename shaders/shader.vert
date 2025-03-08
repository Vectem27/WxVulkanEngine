#version 450
layout(location = 0) in vec2 inPosition; // Position du vertex en 2D
layout(location = 1) in vec3 inColor;    // Couleur du vertex

layout(location = 0) out vec3 fragColor; // Couleur passée au fragment shader

void main() {
    gl_Position = vec4(inPosition, 0.0, 1.0); // Position en 2D
    fragColor = inColor; // Passe la couleur au fragment shader
}