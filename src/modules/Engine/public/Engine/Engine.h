#ifndef ENGINE_H
#define ENGINE_H

#include "RenderAPI.h"

class Engine 
{
public:
    static Engine& GetInstance() {
        static Engine instance;
        return instance;
    }

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    Engine(Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;
    
public:
    RenderAPI GetRenderAPI() const { return renderAPI; }

private:
    RenderAPI renderAPI{RenderAPI::Vulkan};

    Engine() = default;
    ~Engine() = default;
};

inline Engine& GetEngine() 
{
    return Engine::GetInstance();
}



#endif // ENGINE_H