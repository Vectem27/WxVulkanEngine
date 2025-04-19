#ifndef IRENDERMATERIAL_H
#define IRENDERMATERIAL_H

#include "Vector.hpp"

namespace RHI
{
    class IRenderTexture;

    class IRenderMaterial
    {
    public:
        virtual ~IRenderMaterial() = default;

        virtual void SetInteger(int value, uint32_t parameterIndex) = 0;
        virtual void SetScalar(float value, uint32_t parameterIndex) = 0;
        virtual void SetVector(Vector4f vector, uint32_t parameterIndex) = 0;
        virtual void SetTexture(IRenderTexture* texture, uint32_t parameterIndex) = 0;
    };
}

#endif // IRENDERMATERIAL_H