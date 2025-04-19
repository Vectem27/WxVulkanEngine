#ifndef IRENDERTEXTURE_H
#define IRENDERTEXTURE_H

#include <inttypes.h>

#include "EngineCore.hpp"

namespace RHI 
{
    class IRenderTexture
    {
    public:
        virtual ~IRenderTexture() = default;

        virtual uint32_t GetWidth() const = 0;       
        virtual uint32_t GetHeight() const = 0;

        virtual uint8_t GetFormat() const = 0;
        
        /**
         * @brief Set the texture size
         * 
         * @param width Texture width to set
         * @param height Texture height to set
         * @note The width and height should be greater or equal to 64.
         */
        virtual void SetSize(uint32_t width, uint32_t height) = 0;

        /**
         * @brief Set the texture format
         * 
         * @param format Texture format to set
         * @note The format should be standardized with the ETextureFormat enum class or StandardTextureFormat struct.
         */
        virtual void SetFormat(uint8_t format) = 0;

        /**
         * @brief Set the texture data
         * 
         * @param data Data array to set
         * @note The data array should be in the format specified by the texture format (uint8, float16, float32)
         * @note The data array should be allocated and managed by the caller.
         * @note The data array size should be equal to the width * height * bytes per pixel.
         */
        virtual void SetData(const void* data) = 0;
    };

}

#endif // IRENDERTEXTURE_H