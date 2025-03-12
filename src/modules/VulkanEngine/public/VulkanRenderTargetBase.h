#ifndef VULKANRENDERTARGETBASE_H
#define VULKANRENDERTARGETBASE_H

#include "IRenderTarget.h"

class VulkanRenderTargetBase : public IRenderTarget
{
public:
    virtual ~VulkanRenderTargetBase() = default;

public:
    virtual bool Init(class IRenderEngine* renderEngine) override;
    virtual void Cleanup() override = 0;

    virtual const VkCommandBuffer& GetCurrentCommandBuffer() const override = 0;
    virtual const VkFence& GetCurrentRenderFence() const = 0;
    virtual const VkFramebuffer& GetCurrentFrameBuffer() const = 0;

    virtual const VkQueue& GetGraphicsQueue() const = 0;
    virtual const VkQueue& GetPresentQueue() const = 0;

    uint32_t GetWidth() const override { return width; }
    uint32_t GetHeight() const override { return height; }

protected:
    void SetWidth(uint32_t width) { this->width = width; }
    void SetHeight(uint32_t height) { this->height = height; }

public: // Render commands
    virtual bool BeginRenderCommands();
    virtual void EndRenderCommands();
    virtual void Present();

    VkClearColorValue clearColor{0.0f,0.0f,0.0f,0.0f};

private:
    uint32_t width{0};
    uint32_t height{0};

protected:
    class VulkanRenderer* renderEngine{nullptr};
};

#endif // VULKANRENDERTARGETBASE_H