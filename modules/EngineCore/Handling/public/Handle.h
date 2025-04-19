#ifndef HANDLE_H
#define HANDLE_H

#include <inttypes.h>
#include <ostream>

using HandleType = uint32_t;

template<class T>
struct Handle 
{
private:
    /**
     * @brief Constructor for Handle. This constructor is private and is supposed to be used only by the HandleManager class.
     * @param handleID The ID of the handle to be created.
     */
    explicit Handle(HandleType handleID) : handleID(handleID) {}
    friend class HandleManager;

public:
    /**
     * @brief Construct a new invalid Handle object
     */
    Handle() : handleID(0) {}

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
    const HandleType handleID;
};

#endif // HANDLE_H