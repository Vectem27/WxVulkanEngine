#ifndef VULKANCAMERA_H
#define VULKANCAMERA_H

#include "ICamera.h"

class VulkanCamera : public ICamera
{
public:
    VulkanCamera() {}
    ~VulkanCamera() {}

public: // ICamera Interface
    virtual bool Init(class IRenderEngine renderEngine) = 0;
    virtual bool Render(class IRenderable renderable) = 0;
    virtual void Cleanup() = 0;
    virtual void SetRenderTarget(class IRenderTarget* renderTarget) = 0;
    virtual class IRenderTarget* GetRenderTarget() const = 0;

};
#endif // VULKANCAMERA_H