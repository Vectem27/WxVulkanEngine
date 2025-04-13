#ifndef ACTORBASE_H
#define ACTORBASE_H

#include "SceneComponent.h"
#include <stdexcept>

class World;

struct ActorCreationInfo
{
    World* world;
};

class ActorBase : public SceneComponent
{
public:
    ActorBase(ActorCreationInfo creationInfo) : creationInfo(creationInfo)
    {
        if (!creationInfo.world)
            throw std::runtime_error("Invalid world");

        UpdateWorldTransform();
    }
    virtual ~ActorBase() = default;

public:
    virtual World* GetWorld() const override { return creationInfo.world; }
    ActorCreationInfo creationInfo;
};

#endif // ACTORBASE_H