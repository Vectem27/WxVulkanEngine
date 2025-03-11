#ifndef VERTEX_H
#define VERTEX_H


#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Vertex 
{
    glm::vec3 pos {0.0f, 0.0f, 0.0f}; // Position en 2D
    glm::vec3 color {0.0f, 0.0f, 0.0f}; // Couleur
    glm::vec3 normal {0.0f, 0.0f, 0.0f}; // Normal
    glm::vec2 uv {0.0f, 0.0f}; // uv
};

struct TransformMVP
{
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct ObjectData
{
    alignas(16) glm::mat4 model;
};
#endif // VERTEX_H