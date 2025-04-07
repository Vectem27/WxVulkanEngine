#ifndef IWORLDCONTEXTOBJECT
#define IWORLDCONTEXTOBJECT

class World;

class IWorldContextObject
{
public:
    virtual World* GetWorld() const = 0;
};
#endif // IWORLDCONTEXTOBJECT