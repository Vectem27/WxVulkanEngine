#ifndef CUBE_H
#define CUBE_H

#include "SceneComponent.h"
#include "CubeMesh.h"

class Cube : public SceneComponent
{
public:
    virtual ~Cube()
    {
        if (mesh)
            delete mesh;
    }
public: // IRenderable Interface
    bool Init()
    {
        mesh = new CubeMesh();
        mesh->InitVulkanMesh();
        return true;
    }

    virtual void* GetRenderMesh() const override { return mesh; }
    
public:
    void SetMaterial(IVulkanSurfaceMaterial* material)
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