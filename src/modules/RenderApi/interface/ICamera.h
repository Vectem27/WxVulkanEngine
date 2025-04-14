#ifndef ICAMERA_H
#define ICAMERA_H

class ICamera
{
public:
    virtual void SetRenderTarget(class IRenderTarget* renderTarget) = 0;
    virtual class IRenderTarget* GetRenderTarget() const = 0;
};

#endif // ICAMERA_H