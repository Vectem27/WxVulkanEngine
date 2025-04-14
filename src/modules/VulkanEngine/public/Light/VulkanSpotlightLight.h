#ifndef VULKANPROJECTORLIGHT_H
#define VULKANPROJECTORLIGHT_H

#include "EngineCore.hpp"
#include "IVulkanLight.h"
#include <vulkan/vulkan.h>

class VulkanShadowMapCamera;
class VulkanShadowMap;

class VulkanSpotlightLight : public IVulkanLight
{
public:
    VulkanSpotlightLight();
    virtual ~VulkanSpotlightLight();

public:
    virtual Array<VulkanShadowMapCamera*> GetShadowMapCameras() const override;
    
public:
    const Matrix4f&    GetLightViewProj() const { return viewProj; }
    const Vector3f&    GetLightPosition() const { return position; }
    const Vector3f&    GetLightDirection() const { return direction; }
    const Vector3f&    GetLightColor() const { return color; }
    const float&       GetLightAngle() const { return angle; }
    const float&       GetLightLength() const { return length; }
    const float&       GetLightIntensity() const { return intensity; }
    const float&       GetLightSoftAngle() const { return softAngle; }
    const VkImageView& GetLightShadowMapView() const { return shadowMapView; }

    void SetLightColor(const Vector3f& color) { this->color = color; }
    void SetLightAngle(const float& angle) { this->angle = angle; UpdateCameraProperties(); }
    void SetLightLength(const float& length) { this->length = length; UpdateCameraProperties(); }
    void SetLightIntensity(const float& intensity) { this->intensity = intensity; }
    void SetLightSoftAngle(const float& angle) { this->softAngle = angle; UpdateCameraProperties(); }
    void SetTransform(Transform transform);
    
private:
    void SetShadowMap(VkImageView shadowMapView) { this->shadowMapView = shadowMapView; }
    void UpdateCameraProperties();

private:
    Matrix4f viewProj{Matrix4f(1.0f)};    // Matrice vue/projection (assure l'alignement sur 16 octets)
    Vector3f position{0.0f};              // Position de la lumière (assure l'alignement sur 16 octets)
    Vector3f direction{1.0f, 0.0f, 0.0f}; // Direction de la lumière
    Vector3f color{1.0f, 1.0f, 1.0f};     // Couleur de la lumière
    float    angle{ToRadians(30.0f)};     // Angle de la lumière en radians
    float    length{100.0f};              // Longueur du cône lumineux (si nécessaire)
    float    intensity{1.0f};             // Intensité de la lumière
    float    softAngle{ToRadians(1.0f)};
    VkImageView shadowMapView{VK_NULL_HANDLE};

    VulkanShadowMapCamera* shadowMapCamera{nullptr};
    VulkanShadowMap* shadowMap{nullptr};
};

#endif // VULKANPROJECTORLIGHT_H

