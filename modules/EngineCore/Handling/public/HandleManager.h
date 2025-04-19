#ifndef HANDLEMANAGER_H
#define HANDLEMANAGER_H

#include <unordered_map>
#include <mutex>
#include <memory>

#include "Handle.h"
#include "IHandleable.h"

template<class T>
class HandleManager 
{
private:
    HandleManager() = default;
    ~HandleManager() = default;
public:
    static HandleManager<T>& GetInstance() 
    {
        static HandleManager<T> instance;
        return instance;
    }

    HandleManager(const HandleManager&) = delete;
    HandleManager& operator=(const HandleManager&) = delete;
    HandleManager(HandleManager&&) = delete;
    HandleManager& operator=(HandleManager&&) = delete;

public: 

    /**
     * @brief Create and assign a handle to an object. If the object already has a handle, this function will do nothing.
     * 
     * @param object The object to assign a handle to.
     * @return The handle assigned to the object.
     * @note This function is thread-safe and will generate a unique handle for the object.
     */
    Handle<T> AssignUniqueHandle(std::shared_ptr<IHandleable<T>>& object)
    {
        if (object == nullptr || !object->GetHandle().IsValid())
            return Handle<T>(HandleType(0)); // Return an invalid handle if the object is null
        
        // Lock the handle map to prevent concurrent access on map modifications
        std::lock_guard<std::mutex> lock(handleMapMutex); 
        
        Handle<T> handle(GenerateHandleID());

        registredHandles.insert(handle, object)
        object.SetHandle(handle);
    }

    /**
     * @brief Get an object from its handle.
     * 
     * @param handle The handle of the object to retrieve.
     * @return An optional containing the object if it exists, or an empty optional if it doesn't.
     * @note This function is thread-safe and will return a nullptr if the object has been destroyed or doesnt exist.
     */
    std::shared_ptr<IHandleable<T>> GetObjectFromHandle(Handle<T> handle) const
    {
        std::lock_guard<std::mutex> lock(handleMapMutex); 

        auto it = registredHandles.find(handle);
        if (it != registredHandles.end())
            return it->second.lock();

        return nullptr;
    }

private: // Handle Instanciation
    HandleType GenerateHandleID()
    {
        std::lock_guard<std::mutex> lock(handleCreationMutex);
        return HandleType(++currentFirstAvailableHandleID);
    }

    std::mutex handleCreationMutex;

    HandleType currentFirstAvailableHandleID{HandleType(0)};

private: // Objects management
    std::unordered_map<Handle<T>, std::weak_ptr<IHandleable<T>>> registredHandles;
    std::mutex handleMapMutex;
};

template<class T>
inline HandleManager<T>& GetHandleManager()
{
    return HandleManager<T>::GetInstance();
}

#endif // HANDLEMANAGER_H
