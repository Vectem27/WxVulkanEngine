#ifndef PROJECTORLIGHTCOMPONENT_H
#define PROJECTORLIGHTCOMPONENT_H

#include "VulkanProjectorLight.h"

class ProjectorLightComponent : public SceneComponent, public VulkanSpotlightLight
{
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