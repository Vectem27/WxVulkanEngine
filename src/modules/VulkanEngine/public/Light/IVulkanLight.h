#ifndef IVULKANLIGHT_H
#define IVULKANLIGHT_H

#include <string>
#include "Array.h"

class VulkanShadowMapCamera;

class IVulkanLight
{
public:
    virtual ~IVulkanLight() = default;

    virtual Array<VulkanShadowMapCamera*> GetShadowMapCameras() const = 0;
};

#endif // IVULKANLIGHT_H