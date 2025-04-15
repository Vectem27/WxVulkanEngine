#ifndef IVULKANSURFACEMATERIAL_H
#define IVULKANSURFACEMATERIAL_H

#include "IVulkanMaterial.h"

class IVulkanSurfaceMaterial : public IVulkanMaterial 
{
public:
    virtual ~IVulkanSurfaceMaterial() = default;

public:
    virtual void BindForShadowMap(VkCommandBuffer commandBuffer) const = 0;

};

#endif // IVULKANSURFACEMATERIAL_H