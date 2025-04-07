#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

class IRenderTarget
{
public:
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
};

#endif // IRENDERTARGET_H