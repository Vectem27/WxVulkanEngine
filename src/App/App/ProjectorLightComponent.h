#ifndef PROJECTORLIGHTCOMPONENT_H
#define PROJECTORLIGHTCOMPONENT_H

#include "VulkanSpotlightLight.h"

class ProjectorLightComponent : public SceneComponent, public VulkanSpotlightLight
{
public:
public:
    virtual void UpdateWorldTransform() override
    {
        SceneComponent::UpdateWorldTransform();
        VulkanSpotlightLight::SetTransform(GetWorldTransform());
    }
    
    virtual void CollectAllRenderChilds(Array<IRenderable*>& childs, ERenderPassType pass) override
    {
        childs.Add(this);
        SceneComponent::CollectAllRenderChilds(childs, pass);
    }
};

#endif // PROJECTORLIGHTCOMPONENT_H