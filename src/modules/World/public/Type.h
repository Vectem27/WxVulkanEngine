#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <array>

struct Transform
{
    Vector location { 0.0f, 0.0f, 0.0f };
    Rotator rotation { 0.0f, 0.0f, 0.0f };
    Vector scale { 0.0f, 0.0f, 0.0f };
};

struct Vector
{
    double x{ 0.0f };
    double y{ 0.0f };
    double z{ 0.0f };
};

struct Rotator
{
    double pitch{ 0.0f };
    double roll{ 0.0f };
    double yaw{ 0.0f };
};

#endif // TRANSFORM_H