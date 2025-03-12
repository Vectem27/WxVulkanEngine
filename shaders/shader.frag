#version 450
layout(location = 0) in vec3 fragColor; // Couleur reçue du vertex shader
layout(location = 1) in vec3 fragNormal; // Couleur reçue du vertex shader
layout(location = 2) in vec2 fragUV; // Couleur reçue du vertex shader
layout(location = 0) out vec4 outColor; // Couleur de sortie

void main() 
{
    outColor = vec4(fragColor, 1.0); // Applique la couleur
}