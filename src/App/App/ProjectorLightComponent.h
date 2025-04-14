#ifndef PROJECTORLIGHTCOMPONENT_H
#define PROJECTORLIGHTCOMPONENT_H

#include "VulkanSpotlightLight.h"
#include "ILightComponent.h"
#include "Vector.hpp"

class SpotlightLightComponent : public SceneComponent, public ILightComponent
{
public:
    SpotlightLightComponent()
    {
        vulkanLight = new VulkanSpotlightLight(); 
        SetLightColor({1.0f, 1.0f, 1.0f});
        SetLightAngle(30.f);
        SetLightSoftAngle(1.0f);
        SetLightLength(100.0f);
        SetLightIntensity(1.0f);
    }
public:
    virtual void UpdateWorldTransform() override
    {
        SceneComponent::UpdateWorldTransform();
        vulkanLight->SetTransform(GetWorldTransform());
    }
    
    virtual void CollectAllRenderChilds(Array<IRenderable*>& childs, ERenderPassType pass) override
    {
        childs.Add(this);
        SceneComponent::CollectAllRenderChilds(childs, pass);
    }

    virtual void* GetRenderLight(RenderAPI renderAPI) override
    {
        if (renderAPI == RenderAPI::Vulkan)
            return vulkanLight;
        return nullptr;
    }

    inline void SetLightColor(const Vector3f& color) 
    { 
        lightColor = color; 
        if(vulkanLight)
            vulkanLight->SetLightColor(color);
    }

    void SetLightAngle(const float& angle) 
    {
        lightAngle = angle; 
        if(vulkanLight)
            vulkanLight->SetLightAngle(ToRadians(angle));
    }

    void SetLightLength(const float& length) 
    { 
        lightLength = length; 
        if(vulkanLight)
            vulkanLight->SetLightLength(length);
    }

    void SetLightIntensity(const float& intensity) 
    { 
        lightIntensity = intensity; 
        if(vulkanLight)
            vulkanLight->SetLightIntensity(intensity);
    }

    void SetLightSoftAngle(const float& angle) 
    { 
        lightSoftAngle = angle; 
        if(vulkanLight)
            vulkanLight->SetLightSoftAngle(ToRadians(angle));
    }
    
private: // Light parameters
    Vector3f lightColor;
    float lightAngle;
    float lightSoftAngle;
    float lightIntensity;
    float lightLength;

private: // Render lights
    VulkanSpotlightLight* vulkanLight;
};

#endif // PROJECTORLIGHTCOMPONENT_H