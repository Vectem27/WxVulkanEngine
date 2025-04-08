#ifndef VULKANPROJECTORLIGHT_H
#define VULKANPROJECTORLIGHT_H

#include "EngineCore.hpp"
#include <vulkan/vulkan.h>

class VulkanCamera;
class VulkanRenderEngine;
class VulkanRenderTarget;

struct ShadowMapInfo
{
    VkImageView imageView;
    VkSampler sampler;
};

struct ProjectorLightData
{
    Matrix4f viewProj{Matrix4f(1.0f)};    // Matrice vue/projection (assure l'alignement sur 16 octets)
    Vector3f position{0.0f};              // Position de la lumière (assure l'alignement sur 16 octets)
    Vector3f direction{1.0f, 0.0f, 0.0f}; // Direction de la lumière
    Vector3f color{1.0f, 1.0f, 1.0f};     // Couleur de la lumière
    float    angle{0.0f};                 // Angle de la lumière en radians
    float    length{100.0f};              // Longueur du cône lumineux (si nécessaire)
    float    intensity{1.0f};             // Intensité de la lumière
    Array<ShadowMapInfo> shadowMapsInfo;
};

class VulkanSpotlightLight
{
public:
    ~VulkanSpotlightLight();
    void InitVulkanSpotlightLight(VulkanRenderEngine* renderEngine);
public:
    ProjectorLightData GetSpotlightLightData() const;

    void SetTransform(Transform transform);
    void SetShadowMap(VkImageView shadowMapView, VkSampler shadowMapSampler)
    {
        data.shadowMapsInfo.Insert({shadowMapView, shadowMapSampler}, 0, true);
    }
    
private:
    ProjectorLightData data{};
public:
    VulkanCamera* camera;
    VulkanRenderTarget* renderTarget;
};

#endif // VULKANPROJECTORLIGHT_H

