#ifndef CUBE_H
#define CUBE_H

#include "SceneComponent.h"
#include "CubeMesh.h"

class Cube : public SceneComponent
{
public: // IRenderable Interface
    bool Init(IRenderEngine* engine)
    {
        mesh = new CubeMesh();
        mesh->InitVulkanMesh(reinterpret_cast<VulkanRenderEngine*>(engine));
        return true;
    }

    virtual bool ShouldRenderInPass(ERenderPassType pass) const override { return true; }

    virtual void CollectAllRenderChilds(Array<const IRenderable*>& childs, ERenderPassType pass) const override
    {
        childs.Add(this);
        for (const auto& child : GetChilds())
            child->CollectAllRenderChilds(childs, pass);
            
    }
    virtual BoundingBox GetRenderBoundingBox() const override { return BoundingBox(); }

    virtual void* GetRenderMesh() const override { return mesh; }


public:
    void SetMaterial(IVulkanMaterial* material)
    {
        mesh->SetMaterial(material);
    }
public:
    virtual void UpdateWorldTransform() override
    {
        SceneComponent::UpdateWorldTransform();
        mesh->SetMeshTransform(GetWorldTransform());
    }

private: 
    CubeMesh* mesh;

};
#endif // CUBE_H