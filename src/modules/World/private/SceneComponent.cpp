#include "SceneComponent.h"
#include <algorithm>

void SceneComponent::AddChild(SceneComponent *child)
{
    if (!HasChild(child))
        childs.push_back(child);
    child->SetParent(this);
}

void SceneComponent::RemoveChild(SceneComponent *child)
{
    childs.erase(std::remove(childs.begin(), childs.end(), child), childs.end());
    if (child->GetParent() == this)
        child->SetParent(nullptr);
}

bool SceneComponent::HasChild(SceneComponent *inChild) const
{
    for (const auto& child : GetChilds())
    {
        if (inChild == child)
            return true;
    }
    return false;
}

void SceneComponent::SetRelativeTransform(const Transform &transform)
{
    relativeTransform = transform;
    UpdateWorldTransform();
}

void SceneComponent::SetRelativePosition(const Vector3f &pos)
{
    relativeTransform.position = pos;
    UpdateWorldTransform();
}

void SceneComponent::SetRelativeRotation(const Rotator &rot)
{
    relativeTransform.rotation = rot;
    UpdateWorldTransform();
}

void SceneComponent::SetRelativeScale(const Vector3f &scale)
{
    relativeTransform.scale = scale;
    UpdateWorldTransform();
}


void SceneComponent::UpdateWorldTransform()
{
    if (GetParent())
        worldTransform = GetRelativeTransform().RelativeTo(GetParent()->GetWorldTransform());
    else    
        worldTransform = GetRelativeTransform();

    for(const auto& child : GetChilds())
    {
        child->UpdateWorldTransform();
    }
}
