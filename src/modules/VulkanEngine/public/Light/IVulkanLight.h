#ifndef IVULKANLIGHT_H
#define IVULKANLIGHT_H

#include <string>

struct VulkanLightType
{
    std::string type;

    bool operator==(const VulkanLightType& other) const
    {
        return type == other.type;
    }

    bool operator!=(const VulkanLightType& other) const
    {
        return type != other.type;
    }
};

class IVulkanLight
{
public:
    virtual VulkanLightType GetLightType() const = 0;
};

#endif // IVULKANLIGHT_H