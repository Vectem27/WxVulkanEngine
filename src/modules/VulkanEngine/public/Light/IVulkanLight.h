#ifndef IVULKANLIGHT_H
#define IVULKANLIGHT_H

#include <string>

struct LightType
{
    std::string type;

    bool operator==(const LightType& other) const
    {
        return type == other.type;
    }
};

class IVulkanLight
{
    virtual LightType GetLightType() const = 0;
};

#endif // IVULKANLIGHT_H