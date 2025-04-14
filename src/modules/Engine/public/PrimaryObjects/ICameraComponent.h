#ifndef ICAMERACOMPONENT_H
#define ICAMERACOMPONENT_H

#include "RenderAPI.h"

class ICameraComponent 
{
public:
    virtual ~ICameraComponent() = default;

    virtual void* GetRenderCamera(RenderAPI renderAPI) = 0;

};

#endif // ICAMERACOMPONENT_H