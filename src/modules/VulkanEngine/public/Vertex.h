#ifndef VERTEX_H
#define VERTEX_H


#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Vertex 
{
    glm::vec3 pos; // Position en 2D
    glm::vec3 color; // Couleur
    //glm::vec3 normal; // Normal
    //glm::vec2 uv; // uv
};

struct ViewData
{
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};
#endif // VERTEX_H