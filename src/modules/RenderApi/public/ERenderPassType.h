#ifndef ERENDERPASSTYPE_H
#define ERENDERPASSTYPE_H

#include <inttypes.h>

enum ERenderPassType : uint32_t
{
    RENDER_PASS_TYPE_DEFAULT = 0b0,
    RENDER_PASS_TYPE_SHADOWMAP = 0b1,
};

#endif // ERENDERPASSTYPE_H