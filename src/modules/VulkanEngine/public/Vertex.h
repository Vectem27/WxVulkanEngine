#ifndef VERTEX_H
#define VERTEX_H


#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "EngineCore.hpp"


struct ViewProj
{
    alignas(16) Matrix4f view;
    alignas(16) Matrix4f proj;
};

struct ObjectData
{
    alignas(16) Matrix4f model;
};
#endif // VERTEX_H