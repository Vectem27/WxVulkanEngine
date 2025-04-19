#ifndef ENGINECORE_HPP
#define ENGINECORE_HPP

#include "Converter.hpp"
#include "Array.h"
#include "BoundingBox.h"
#include "Logger.h"

using CoordinateType = float;

using Position = Vector3<CoordinateType>;
using Rotation = Quaternion<CoordinateType>;
using Scale = Vector3<CoordinateType>;

using TransformMatrix = Matrix4<CoordinateType>;

using Vector2D = Vector2<CoordinateType>;
using Vector3D = Vector3<CoordinateType>;
using Vector4D = Vector4<CoordinateType>;

using Matrix4D = Matrix4<CoordinateType>;

using LinearColor = Vector4D;

struct Transform
{
    Position position{0.0, 0.0, 0.0};
    Rotation rotation{0.0, 0.0, 0.0, 1.0};
    Scale scale{1.0, 1.0, 1.0};

    TransformMatrix GetTransformMatrix(bool ignoreScale = false) const;

    Transform RelativeTo(Transform base);
};

struct Vertex 
{
    Vector3D pos {0.0f, 0.0f, 0.0f}; // Position en 2D
    Vector3D color {0.0f, 0.0f, 0.0f}; // Couleur
    Vector3D normal {0.0f, 0.0f, 0.0f}; // Normal
    Vector2D uv {0.0f, 0.0f}; // uv
};

#endif // ENGINECORE_HPP