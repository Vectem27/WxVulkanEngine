#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include "RenderAPI.h"

class IRenderTarget 
{
public:
    virtual ~IRenderTarget() = default;

    virtual void* GetRenderTarget(RenderAPI renderAPI) = 0;
};

#endif // IRENDERTARGET_H