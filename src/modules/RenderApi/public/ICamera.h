#ifndef ICAMERA_H
#define ICAMERA_H

class ICamera
{
public:
    virtual bool Init(class IRenderEngine* renderEngine, class IRenderTarget* renderTarget = nullptr) = 0;
    //virtual bool Render(class IRenderable* renderable) = 0;
    virtual void Cleanup() = 0;
public:
    virtual void SetRenderTarget(class IRenderTarget* renderTarget) = 0;
    virtual class IRenderTarget* GetRenderTarget() const = 0;
};

#endif // ICAMERA_H