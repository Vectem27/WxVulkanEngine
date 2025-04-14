#ifndef ILIGHTCOMPONENT
#define ILIGHTCOMPONENT

#include "RenderAPI.h"

class ILightComponent
{
public:
    virtual void* GetRenderLight(RenderAPI renderAPI) = 0;
    virtual ~ILightComponent() = default;
};

#endif // ILIGHTCOMPONENT