#ifndef ETEXTUREFORMAT_H
#define ETEXTUREFORMAT_H

#include <inttypes.h>
#include <assert.h>

namespace RHI 
{
    struct StandardTextureFormat
    {
        uint8_t channels : 2; // Number of channels (0 = R, 1 = RG, 2 = RGB, 3 = RGBA)
        uint8_t bits : 2;     // Number of bits per channel (0 = 8 bits, 1 = 16 bits, 2 = 32 bits)
        uint8_t type : 3;     // Type of data (0 = UNORM, 1 = SFLOAT, 2 = UINT, 3 = SINT, 4 = SNORM)
        const uint8_t isSpecialFormat : 1 {0}; // Special format flag, if one use the 7 other bits as a bitfield

        consteval operator uint8_t() const
        {
            return (channels) | (bits << 2) | (type << 4) | (isSpecialFormat << 7);
        }
    };

    static_assert(sizeof(StandardTextureFormat) <= 1, "StandardTextureFormat size exceeds 1 byte");
    static_assert(StandardTextureFormat(2, 1, 5) == 0b1010110, "StandardTextureFormat bitfield layout is incorrect");

    /**
     * @brief Enum class representing various texture formats.
     * 
     * The enum values are organized by the number of channels and their types.
     * UNORM formats are unsigned normalized integer formats, while SFLOAT formats are signed float formats.
     * Each format is represented by a unique value for easy identification and usage in rendering operations.
     * 
     * @enum ETextureFormat
     * @ingroup RHI
     * 
     * @author Vectem
     */
    enum class ETextureFormat : uint8_t
    {
        // UNORM
        // 8 bits per channel

        /* 8 bit normalized grayscale texture */
        R8 = StandardTextureFormat(0, 0, 0),
        /* 8 bits, normalized two channels (red, green) texture */
        RG8 = StandardTextureFormat(1, 0, 0),
        /* 8 bits, normalized three channels (red, green, blue) texture */
        RGB8 = StandardTextureFormat(2, 0, 0),
        /* 8 bits, normalized four channels (red, green, blue, alpha) texture */
        RGBA8 = StandardTextureFormat(3, 0, 0),

        // SFLOAT
        // 16 bits per channel

        /* 16 bits, signed float grayscale texture */
        R16 = StandardTextureFormat(0, 1, 1),
        /* 16 bits, signed float two channels (red, green) texture */
        RG16 = StandardTextureFormat(1, 1, 1),
        /* 16 bits, signed float three channels (red, green, blue) texture */
        RGB16 = StandardTextureFormat(2, 1, 1),
        /* 16 bits, signed float four channels (red, green, blue, alpha) texture */
        RGBA16 = StandardTextureFormat(3, 1, 1),

        // SFLOAT
        // 32 bits per channel

        /* 32 bits, signed float grayscale texture */
        R32 = StandardTextureFormat(0, 2, 1),
        /* 32 bits, signed float two channels (red, green) texture */
        RG32 = StandardTextureFormat(1, 2, 1),
        /* 32 bits, signed float three channels (red, green, blue) texture */
        RGB32 = StandardTextureFormat(2, 2, 1),
        /* 32 bits, signed float four channels (red, green, blue, alpha) texture */
        RGBA32 = StandardTextureFormat(3, 2, 1),
    };

} // namespace RHI

#endif // ETEXTUREFORMAT_H