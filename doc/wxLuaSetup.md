1) Clone wxLua or donwload from from source (https://github.com/pkulchenko/wxlua.git)

2) Compile wxLua
# MinGW :
 - cd Path/To/wxLua
 - cmake -S . -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM="3.5" -DBUILD_EXAMPLES=OFF -DBUILD_SHARED_LIBS=ON -DwxWidgets_ROOT_DIR="C:/wxInstall" -DwxWidgets_LIB_DIR="C:\wxInstall\lib\gcc_x64_dll" 
 - cmake --build . -j8
 - cmake --install .

# Visual Studio :

# GCC/make (Linux)
