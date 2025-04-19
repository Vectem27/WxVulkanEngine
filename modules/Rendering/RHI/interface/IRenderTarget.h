#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include "IRenderTexture.h"

namespace RHI 
{

class IRenderTarget 
{
public:
    virtual ~IRenderTarget() = default;

    virtual void SetSize(uint32_t width, uint32_t height) = 0;

    virtual uint32_t GetWidth() const = 0;       
    virtual uint32_t GetHeight() const = 0;
};

} // namespace RHI

#endif // IRENDERTARGET_H