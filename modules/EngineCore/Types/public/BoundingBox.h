#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Vector.hpp"
#include <cfloat> // pour FLT_MAX

struct BoundingBox
{
    BoundingBox()
        : min(Vector3f(FLT_MAX)), max(Vector3f(-FLT_MAX))
    {
    }

    BoundingBox(const Vector3f& min, const Vector3f& max)
        : min(min), max(max)
    {
    }

    Vector3f min{};
    Vector3f max{};
    
    Vector3f GetOrigin() const
    {
        return (min + max) * 0.5f;
    }
    Vector3f GetSize() const
    {
        return max - min;
    }

    void ExpandToInclude(const Vector3f& point)
    {
        min = Vector3f::Min(min, point);
        max = Vector3f::Max(max, point);
    }
};

#endif // BOUNDINGBOX_H