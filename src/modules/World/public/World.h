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
    virtual bool Init(class IRenderEngine* engine) override { return true; }
    virtual void Draw(const VkCommandBuffer& commandBuffer, ERenderPassType pass) override
    {
        for (const auto& actor : GetActors())
            actor->Draw(commandBuffer, pass);
    }
private:
    Array<Actor*> actors;
    class IRenderEngine* renderEngine;
};

#endif // WORLD_H