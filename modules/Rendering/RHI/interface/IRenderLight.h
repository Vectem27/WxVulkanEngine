#ifndef IRENDERLIGHT_H
#define IRENDERLIGHT_H

#include "EngineCore.hpp"

namespace RHI
{
    class IRenderLight 
    {
    public:
        virtual ~IRenderLight() = default;

        virtual void SetPosition(const Position& position) = 0;
        virtual void SetColor(const LinearColor& color) = 0;
        virtual void SetIntensity(float intensity) = 0;
        //virtual void SetLightChannel(uint8_t channel) = 0;
        //virtual void SetCastShadow(bool castShadow) = 0;
        
        virtual Position GetPosition() const = 0;
        virtual LinearColor GetColor() const = 0;
        virtual float GetIntensity() const = 0;
        //virtual uint8_t GetLightChannel() const = 0;
        //virtual bool GetCastShadow() const = 0;
    };
} // namespace RHI


#endif // IRENDERLIGHT_H