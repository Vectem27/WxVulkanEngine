#ifndef ERENDERENGINE_H
#define ERENDERENGINE_H

#include <string>

struct RenderAPI
{
    std::string name = "";

    bool operator==(RenderAPI other) const { return other.name == name; }

    static const RenderAPI Vulkan;
};

const RenderAPI RenderAPI::Vulkan{"Vulkan render api"};

inline const RenderAPI VULKAN_API {"Vulkan render api"};

#endif // ERENDERENGINE_H