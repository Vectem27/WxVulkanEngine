#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

#include <vulkan/vulkan.h>

class IRenderTarget
{
public:
    //virtual bool Init(class IRenderEngine* renderEngine) = 0;
    //virtual void Cleanup() = 0;
//
    //virtual const VkCommandBuffer& GetCurrentCommandBuffer() const = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
};

#endif // IRENDERTARGET_H