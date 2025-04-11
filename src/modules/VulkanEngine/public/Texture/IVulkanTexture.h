#ifndef IVULKANTEXTURE_H
#define IVULKANTEXTURE_H

#include <inttypes.h>

/**
 * @interface IVulkanTexture
 */
class IVulkanTexture
{
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
};

#endif // IVULKANTEXTURE_H