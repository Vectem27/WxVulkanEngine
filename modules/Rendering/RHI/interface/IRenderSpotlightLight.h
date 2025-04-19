#ifndef IRENDERSPOTLIGHTLIGHT_H
#define IRENDERSPOTLIGHTLIGHT_H

#include "IRenderLight.h"

namespace RHI 
{
    class IRenderSpotlightLight : public IRenderLight 
    {
    public:
        virtual ~IRenderSpotlightLight() = default;

        virtual void SetDirection(const Vector3D& direction) = 0;
        virtual void SetSpotlightAngle(float angle) = 0;
        virtual void SetSpotlightSoftAngle(float angle) = 0;
        virtual void SetSpotlightRange(float range) = 0;

        virtual const Vector3D& GetDirection() const = 0;
        virtual float GetSpotlightAngle() const = 0;
        virtual float GetSpotlightSoftAngle() const = 0;
        virtual float GetSpotlightRange() const = 0;
    };

} // namespace RHI

#endif // IRENDERSPOTLIGHTLIGHT_H