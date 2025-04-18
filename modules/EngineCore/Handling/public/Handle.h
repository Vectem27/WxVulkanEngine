#ifndef HANDLE_H
#define HANDLE_H

#include <inttypes.h>
#include <ostream>

using HandleType = uint32_t;

template<class T>
struct Handle 
{
private:
    explicit Handle(HandleType handleID) : handleID(handleID) {}
    friend class HandleManager;

public:
    Handle() = delete;

    Handle(const Handle&) = default;
    Handle& operator=(const Handle&) = default;
    Handle(const Handle&&) noexcept = default;
    Handle& operator=(const Handle&&) noexcept = default;
    ~Handle() = default;


public:
    bool operator==(const Handle& other) const
    {
        return handleID == other.handleID;
    }

    bool operator!=(const Handle& other) const
    {
        return handleID != other.handleID;
    }

    friend std::ostream& operator<<(std::ostream& os, const Handle& h) 
    {
        os << h.handleID;
        return os;
    }

    bool IsValid() const
    {
        return handleID != 0;
    }
    
private:
    HandleType handleID;
};

#endif // HANDLE_H