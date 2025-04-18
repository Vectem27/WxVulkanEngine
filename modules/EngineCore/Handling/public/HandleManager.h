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

public: // Handle Instanciation
    Handle<T> AssignUniqueHandle(std::shared_ptr<IHandleable<T>>& object)
    {
        // Lock the handle map to prevent concurrent access on map modifications
        std::lock_guard<std::mutex> lock(handleMapMutex); 
        
        Handle<T> handle(GenerateHandleID());

        registredHandles.insert(handle, object)
        object.SetHandle(handle);
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
