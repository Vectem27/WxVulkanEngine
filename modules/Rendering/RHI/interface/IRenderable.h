#ifndef IRENDERABLE_H
#define IRENDERABLE_H

namespace RHI
{
    class IRenderable
    {
    public:
        virtual ~IRenderable() = default;

        virtual void Render(const void* renderInfo) const = 0;
    };
}

#endif // IRENDERABLE_H