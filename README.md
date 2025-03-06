# WxVulkanEngine
An simple engine using wxWidget, wxLua and Vulkan

Build : 
cmake -S . -B ./build -G "MinGW Makefiles"

Compilation : 
cmake --build ./build -j8

cmake -S . -B ./build -G "MinGW Makefiles" 
cmake --build ./build -j8 
./build/WxVulkanEngine.exe


wxLua build :
cmake -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON .

cmake -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON .




