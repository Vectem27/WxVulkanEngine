#ifndef SCENECOMPONENT_H
#define SCENECOMPONENT_H

#include <vector>
#include "EngineCore.hpp"
#include "IWorldContextObject.h"
#include "IRenderable.h"

class SceneComponent : public IWorldContextObject, public IRenderable
{
public:
    virtual ~SceneComponent()
    {
        for (const auto& child : GetChilds())
            if (child)
                delete child;
    }
public: // IWorldContextObject Interface
    virtual World* GetWorld() const override
    {
        if (!GetParent())
            return nullptr;
        return GetParent()->GetWorld();
    }
    
public: // IRenderable
    virtual bool ShouldRenderInPass(ERenderPassType pass) const override { return false; }

    virtual void CollectAllRenderChilds(Array<IRenderable*>& childs, ERenderPassType pass) override
    {
        if (ShouldRenderInPass(pass))
            childs.Add(this);
        for (const auto& child : GetChilds())
            child->CollectAllRenderChilds(childs, pass);
    }
    virtual BoundingBox GetRenderBoundingBox() const override { return BoundingBox(); }

    void CollectChilds(Array<SceneComponent*>& childs)
    {
        childs.Add(this);
        for (const auto& child : GetChilds())
            child->CollectChilds(childs);
    }

public: // ISceneNode Interface
    virtual void AddChild(SceneComponent* child);
    virtual void RemoveChild(SceneComponent* child);
    bool HasChild(SceneComponent* child) const;

    virtual inline void SetParent(SceneComponent* parent) { this->parent = parent; UpdateWorldTransform(); }

    virtual inline const std::vector<SceneComponent*>& GetChilds() const { return childs; }
    virtual inline SceneComponent* GetParent() const { return parent; }


    virtual void SetRelativeTransform(const Transform& transform);
    virtual void SetRelativePosition(const Vector3f& pos);
    virtual void SetRelativeRotation(const Rotator& rot);
    virtual void SetRelativeScale(const Vector3f& scale);
    virtual Transform GetRelativeTransform() const { return relativeTransform; }
    virtual Transform GetWorldTransform() const { return worldTransform; }

    virtual void UpdateWorldTransform();
protected:
    Transform relativeTransform{};
    Transform worldTransform{};

private:
    SceneComponent* parent{nullptr};
    std::vector<SceneComponent*> childs;
};

#endif // SCENECOMPONENT_H