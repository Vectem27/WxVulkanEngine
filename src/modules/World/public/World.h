#ifndef WORLD_H
#define WORLD_H

#include "SceneComponent.h"
#include "Actor.h"
#include "EngineCore.hpp"

class World
{
public:
    ~World() 
    {
        for(const auto& actor : GetActors())
            if(actor)
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

private:
    Array<Actor*> actors;
};

#endif // WORLD_H