#ifndef WORLD_H
#define WORLD_H

#include "SceneComponent.h"
#include "Actor.h"
#include "EngineCore.hpp"
#include "IRenderable.h"

class World : public IRenderable
{
public:
    ~World() 
    {
        for(const auto& actor : GetActors())
            delete actor;
    }

    Array<Actor*> GetActors() const { return actors; }

    template <class ActorType>
    Actor* SpawnActor(Transform transform = Transform()) 
    {
        static_assert(std::is_base_of<Actor, ActorType>::value, 
            "ActorType must be derived from Actor");

        ActorCreationInfo info;
        info.world = this;
        
        Actor* actor = new ActorType(info);
        actors.Add(actor);
        actor->SetRelativeTransform(transform);
        return actor;
    }

public:
    // Presistent
    virtual bool ShouldRenderInPass(ERenderPassType pass) const override { return false; }

    virtual void CollectAllRenderChilds(Array<IRenderable*>& childs, ERenderPassType pass) override
    {
        for (const auto& actor : GetActors())
            actor->CollectAllRenderChilds(childs, pass);
    }
    virtual BoundingBox GetRenderBoundingBox() const override { return BoundingBox(); }
private:
    Array<Actor*> actors;
    class IRenderEngine* renderEngine;
};

#endif // WORLD_H