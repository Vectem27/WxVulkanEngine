#ifndef CAMERACOMPONENT_H
#define CAMERACOMPONENT_H

#include "SceneComponent.h"
#include "VulkanCamera.h"

class CameraComponent : public SceneComponent, public VulkanCamera
{
public:
    virtual void UpdateWorldTransform() override
    {
        SceneComponent::UpdateWorldTransform();
        VulkanCamera::SetCameraTransform(GetWorldTransform());
    }
};

#endif // CAMERACOMPONENT_H