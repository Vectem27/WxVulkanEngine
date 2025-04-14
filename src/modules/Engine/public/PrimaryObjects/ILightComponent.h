#ifndef ILIGHTCOMPONENT
#define ILIGHTCOMPONENT

#include "RenderAPI.h"

class ILightComponent
{
public:
    virtual ~ILightComponent() = default;

    virtual void* GetRenderLight(RenderAPI renderAPI) = 0;
};

#endif // ILIGHTCOMPONENT