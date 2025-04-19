#ifndef IRENDERCAMERA_H
#define IRENDERCAMERA_H

#include "EngineCore.hpp"

namespace RHI
{
    class IRenderCamera 
    {
    public:
        virtual ~IRenderCamera() = default;

        virtual void SetPosition(Position position) = 0;
        virtual void SetRotation(Rotation rotation) = 0;

        virtual Position GetPosition() const = 0;
        virtual Rotation GetRotation() const = 0;
    };
}

#endif // IRENDERCAMERA_H