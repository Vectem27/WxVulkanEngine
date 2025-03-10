#ifndef IRENDERTARGET_H
#define IRENDERTARGET_H

class IRenderTarget
{
public:
    virtual bool Init(class IRenderEngine renderEngine) = 0;
    virtual void Cleanup() = 0;
};

#endif // IRENDERTARGET_H