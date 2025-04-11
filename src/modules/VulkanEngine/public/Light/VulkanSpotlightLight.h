#ifndef VULKANPROJECTORLIGHT_H
#define VULKANPROJECTORLIGHT_H

#include "EngineCore.hpp"
#include "IVulkanLight.h"
#include <vulkan/vulkan.h>

class VulkanCamera;
class VulkanRenderEngine;
class VulkanShadowMapRenderTarget;

struct ProjectorLightData
{
    Matrix4f viewProj{Matrix4f(1.0f)};    // Matrice vue/projection (assure l'alignement sur 16 octets)
    Vector3f position{0.0f};              // Position de la lumière (assure l'alignement sur 16 octets)
    Vector3f direction{1.0f, 0.0f, 0.0f}; // Direction de la lumière
    Vector3f color{1.0f, 1.0f, 1.0f};     // Couleur de la lumière
    float    angle{ToRadians(30.0f)};                 // Angle de la lumière en radians
    float    length{100.0f};              // Longueur du cône lumineux (si nécessaire)
    float    intensity{1.0f};             // Intensité de la lumière
    float    softAngle{ToRadians(1.0f)};
    VkImageView shadowMapView{VK_NULL_HANDLE};
};

class VulkanSpotlightLight : public IVulkanLight
{
public:
    ~VulkanSpotlightLight();
    void InitVulkanSpotlightLight(VulkanRenderEngine* renderEngine);

public: // IVulkanLight Interface
    virtual VulkanLightType GetLightType() const override { return lightType; }

public:
    ProjectorLightData GetSpotlightLightData() const;

    void SetTransform(Transform transform);
    void SetShadowMap(VkImageView shadowMapView) { data.shadowMapView = shadowMapView; }
    
public:
    inline const Matrix4f&    GetLightViewProj() const { return data.viewProj; }
    inline const Vector3f&    GetLightPosition() const { return data.position; }
    inline const Vector3f&    GetLightDirection() const { return data.direction; }
    inline const Vector3f&    GetLightColor() const { return data.color; }
    inline const float&       GetLightAngle() const { return data.angle; }
    inline const float&       GetLightLength() const { return data.length; }
    inline const float&       GetLightIntensity() const { return data.intensity; }
    inline const float&       GetLightSoftAngle() const { return data.softAngle; }
    inline const VkImageView& GetLightShadowMapView() const { return data.shadowMapView; }

    inline void SetLightColor(const Vector3f& color) { data.color = color; }
    inline void SetLightAngle(const float& angle) { data.angle = angle; UpdateCameraProperties(); }
    inline void SetLightLength(const float& length) { data.length = length; UpdateCameraProperties(); }
    inline void SetLightIntensity(const float& intensity) { data.intensity = intensity; }
    inline void SetLightSoftAngle(const float& angle) { data.softAngle = angle; UpdateCameraProperties(); }
    
private:
    void UpdateCameraProperties();
    ProjectorLightData data{};
public:
    VulkanCamera* camera;
    VulkanShadowMapRenderTarget* renderTarget;

    static const VulkanLightType lightType;
};

#endif // VULKANPROJECTORLIGHT_H

