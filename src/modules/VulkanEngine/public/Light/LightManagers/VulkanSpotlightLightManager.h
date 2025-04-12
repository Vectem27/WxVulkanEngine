#ifndef VULKANSPOTLIGHTLIGHTMANAGER_H
#define VULKANSPOTLIGHTLIGHTMANAGER_H

#include <vulkan/vulkan.h>

#include "Array.h"
#include "Matrix.hpp"

#include "IVulkanLightManager.h"
#include "VulkanUniformBuffer.h"

class VulkanSpotlightLight;
class VulkanSpotlightLightPipeline;

class VulkanSpotlightLightManager : public IVulkanLightManager
{
    struct alignas(256) LightData
    {
        alignas(16) Matrix4f viewProj;
        alignas(16) Vector3f pos;
        alignas(16) Vector3f direction;
        alignas(4)  float length;
        alignas(4)  float angle;
        alignas(16) Vector3f color;
        alignas(4)  float intensity;
        alignas(4)  float softAngle; // Angle de transition douce
        alignas(4)  unsigned int shadowMapIndex;
    };
public:
    VulkanSpotlightLightManager(unsigned short maxNumOfLights) 
        : maxNumOfLights(maxNumOfLights) {}

    ~VulkanSpotlightLightManager()
    {
        lightDataBuffer.Cleanup();
    }

public: // IVulkanLightManager Interface
    virtual void InitLightManager();

    virtual void AddLight(const IVulkanLight* light) override;
    virtual void ClearLights() override { lights.Clear(); }

    virtual void Update() override;
    virtual void Bind(VkCommandBuffer commandBuffer) const override;
public:
    unsigned short GetMaxNumOfLights() const { return maxNumOfLights; }
    unsigned short GetLightCount() const { return lights.GetSize();}

    const VkDescriptorSet& GetDescriptorSet() const { return descriptorSet; }
private:
    void CreateDescriptorPool();
    void CreateDescriptorSets();
private:
    unsigned short maxNumOfLights;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;

    Array<const VulkanSpotlightLight*> lights;

    VulkanUniformBuffer lightDataBuffer;
};

#endif // VULKANSPOTLIGHTLIGHTMANAGER_H