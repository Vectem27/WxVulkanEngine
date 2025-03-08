# WxVulkanEngine
An simple engine using wxWidget, wxLua and Vulkan

Build : 
cmake -S . -B ./build -G "MinGW Makefiles"

Compilation : 
cmake --build ./build -j8

cmake -S . -B ./build -G "MinGW Makefiles" 
cmake --build ./build -j8 
./build/WxVulkanEngine.exe



wxWidget Build :
cmake . -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=Release -DwxBUILD_SHARED=ON -G "MSYS Makefiles"
cmake . -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=Debug -DwxBUILD_SHARED=ON -G "MSYS Makefiles"

cmake --build . --config Release -j8
cmake --build . --config Debug -j8



wxLua build :
cmake -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON .

cmake -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON .


cmake -G "MSYS Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DwxBUILD_SAMPLES=OFF .

cmake -G "MSYS Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON .




