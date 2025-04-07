#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "SceneComponent.h"
#include "VulkanLight.h"

class LightComponent : public SceneComponent, public VulkanLight
{
public:
    virtual void UpdateWorldTransform() override
    {
        SceneComponent::UpdateWorldTransform();
        VulkanLight::SetLightTransform(GetWorldTransform());
    }
};

#endif // LIGHTCOMPONENT_H