#ifndef ACTOR_H
#define ACTOR_H

#include "ActorBase.h"

class Actor : public ActorBase
{
private:
    Actor(const ActorCreationInfo& creationInfo) : ActorBase(creationInfo) {}
    virtual ~Actor() = default;
    friend class World;
public:
};

#endif // ACTOR_H