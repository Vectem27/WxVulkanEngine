#ifndef IRENDERENGINE_H
#define IRENDERENGINE_H

class IRenderEngine
{
public:
    virtual bool Init(void* windowHandle = nullptr) = 0;
    virtual void Shutdown() = 0;
};
#endif // IRENDERENGINE_H