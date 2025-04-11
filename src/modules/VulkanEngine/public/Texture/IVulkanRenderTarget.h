#ifndef IVULKANRENDERTARGET_H
#define IVULKANRENDERTARGET_H

/**
 * @interface IVulkanRenderTarget
 */
class IVulkanRenderTarget
{
public:
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
};

#endif // IVULKANRENDERTARGET_H