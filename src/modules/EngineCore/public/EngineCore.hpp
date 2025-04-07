#ifndef ENGINECORE_HPP
#define ENGINECORE_HPP

#include "Converter.hpp"
#include "Array.h"

typedef Quaternion<float> Rotator;

struct Transform
{
    Vector3f position{0.0, 0.0, 0.0};
    Rotator rotation{0.0, 0.0, 0.0, 1.0};
    Vector3f scale{1.0, 1.0, 1.0};

    Matrix4f GetTransformMatrix(bool ignoreScale = false) const;

    Transform RelativeTo(Transform base);
};

struct Vertex 
{
    Vector3f pos {0.0f, 0.0f, 0.0f}; // Position en 2D
    Vector3f color {0.0f, 0.0f, 0.0f}; // Couleur
    Vector3f normal {0.0f, 0.0f, 0.0f}; // Normal
    Vector2f uv {0.0f, 0.0f}; // uv
};

#endif // ENGINECORE_HPP