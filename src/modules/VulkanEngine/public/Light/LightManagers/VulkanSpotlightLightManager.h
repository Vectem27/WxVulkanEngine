#ifndef VULKANSPOTLIGHTLIGHTMANAGER_H
#define VULKANSPOTLIGHTLIGHTMANAGER_H

#include <vulkan/vulkan.h>

#include "Array.h"
#include "Matrix.hpp"

#include "IVulkanLightManager.h"
#include "ShaderStorageBuffer.h"
#include "UniformBufferObject.h"

struct VertexShaderLightData
{
    alignas(16) Matrix4f viewProj;
};

struct FragmentShaderLightData
{
    alignas(16) Vector3f pos;
    alignas(16) Vector3f direction;
    alignas(4) float length;
    alignas(4) float angle;
};

struct BufferData
{
    alignas(4) unsigned int numLights;
    void* data;
};

struct LightsBuffer
{
    UniformBuffer vert;
    UniformBuffer frag;
};

class VulkanRenderEngine;
class VulkanSpotlightLight;

class VulkanSpotlightLightManager : public IVulkanLightManager
{
public:
    VulkanSpotlightLightManager(unsigned short maxNumOfLights) 
        : maxNumOfLights(maxNumOfLights) {}

    ~VulkanSpotlightLightManager()
    {
        lightsBuffer.frag.Cleanup();
        lightsBuffer.vert.Cleanup();
    }

public: // IVulkanLightManager Interface
    virtual void InitLightManager(VulkanRenderEngine* vulkanRenderEngine) override;
    virtual bool IsInitialized() const override { return isInitialized; }

    virtual void AddLight(const IVulkanLight* light) override;
    virtual void ClearLights() override { lights.Clear(); }

    virtual void Update() override;
    virtual void Bind(VkCommandBuffer commandBuffer) const override;
public:
    unsigned short GetMaxNumOfLights() const { return maxNumOfLights; }
    unsigned short GetLightCount() const { return lights.GetSize();}

    VulkanRenderEngine* GetRenderEngine() const { return vulkanRenderEngine; }
    const VkDescriptorSet& GetDescriptorSet() const { return descriptorSet; }
private:
    void CreateDescriptorPool();
    void CreateDescriptorSets();
private:
    unsigned short maxNumOfLights;
    bool isInitialized{false};
    VulkanRenderEngine* vulkanRenderEngine;

    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;

    Array<const VulkanSpotlightLight*> lights;

    LightsBuffer lightsBuffer;
};

#endif // VULKANSPOTLIGHTLIGHTMANAGER_H