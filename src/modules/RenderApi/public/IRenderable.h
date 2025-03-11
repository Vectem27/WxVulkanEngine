#ifndef IRENDERABLE_H
#define IRENDERABLE_H

class IRenderable
{
public:
    virtual bool Init(class IRenderEngine* engine) = 0;
    virtual void draw(class ICamera* camera) = 0;
};

#endif // IRENDERABLE_H
