#ifndef ERENDERENGINE_H
#define ERENDERENGINE_H

#include <string>

struct RenderAPI
{
    std::string name;

    bool operator==(RenderAPI other) const { return other.name == name; }
};

inline const RenderAPI VULKAN_API {"Vulkan render api"};

#endif // ERENDERENGINE_H