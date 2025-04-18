#ifndef IHANDLEABLE_H
#define IHANDLEABLE_H

#include "Handle.h"

template<class T>
class IHandleable 
{
    friend class HandleManager<T>;
protected: 
    void SetHandle(Handle<T> handle) noexcept;
public:
    virtual ~IHandleable() = default;

    virtual Handle<T> GetHandle() noexcept const = 0;
};

#endif // IHANDLEABLE_H