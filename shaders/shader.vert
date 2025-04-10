#version 450

struct FragInfo
{
    vec3 color;
    vec3 normal;
    vec2 uv;
    vec3 worldPosition;
    vec3 worldNormal;
};


layout(location = 0) in vec3 inPosition;  // Position du vertex en 2D
layout(location = 1) in vec3 inColor;     // Couleur du vertex
layout(location = 2) in vec3 inNormal;    // Couleur du vertex
layout(location = 3) in vec2 inUV;        // Couleur du vertex

layout(set = 0, binding = 0) uniform CameraBuffer
{
    mat4 view;
    mat4 proj;
} camera;

layout(set = 1, binding = 0) uniform ObjectBuffer
{
    mat4 model;
} object;

layout(location = 0) out FragInfo fragInfo; 

void main() 
{
    vec4 worldPosition = object.model * vec4(inPosition, 1.0);
    vec4 pos = camera.proj * camera.view * worldPosition; // Position en 2D
    gl_Position = pos;
    fragInfo.uv = inUV;
    fragInfo.normal = inNormal;
    fragInfo.color = inColor; // Passe la couleur au fragment shader
    fragInfo.worldPosition = vec3(worldPosition);

    mat3 normalMatrix = transpose(inverse(mat3(object.model)));  // Inverse transposée de la matrice modèle
    fragInfo.worldNormal = normalize(normalMatrix * inNormal);    // Transformation de la normale en espace monde
}