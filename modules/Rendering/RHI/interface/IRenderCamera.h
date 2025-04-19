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

        virtual void SetFOV(float fov) = 0;
        virtual void SetAspectRatio(float aspectRatio) = 0;
        virtual void SetNearPlane(float nearPlane) = 0;
        virtual void SetFarPlane(float farPlane) = 0;

        virtual Position GetPosition() const = 0;
        virtual Rotation GetRotation() const = 0;

        virtual float GetFOV() const = 0;
        virtual float GetAspectRatio() const = 0;
        virtual float GetNearPlane() const = 0;
        virtual float GetFarPlane() const = 0;
    };
}

#endif // IRENDERCAMERA_H