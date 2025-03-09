# WxVulkanEngine
An simple engine using wxWidget, wxLua and Vulkan

Build : 
cmake -S . -B ./build -G "MinGW Makefiles" 

Compilation : 

cmake -S . -B ./build -G "MinGW Makefiles" -DBUILD_App=ON -DBUILD_CompilationTest=OFF -DCOMPILE_SHADERS=ON
cmake -S . -B ./build -G "MSYS Makefiles" -DBUILD_App=ON -DBUILD_CompilationTest=OFF -DCOMPILE_SHADERS=ON
cmake --build ./build -j8 --config Debug  
cd bin/Debug/WxVulkanEngine ; ./WxVulkanEngine.exe ; cd ../../../


wxWidget Build :
cmake . -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=Release -DwxBUILD_SHARED=ON -G "MSYS Makefiles"
cmake . -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_BUILD_TYPE=Debug -DwxBUILD_SHARED=ON -G "MinGW Makefiles"

cmake --build . --config Release -j8
cmake --build . --config Debug -j8


To test if don't work : -DwxWidgets_DEBUG=1 --debug-find


wxLua build :
cmake -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON .

cmake -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON .


cmake -G "MSYS Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DwxBUILD_SAMPLES=OFF .

cmake -G "MSYS Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DwxWidgets_COMPONENTS="stc;html;aui;adv;core;net;base" -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON .




