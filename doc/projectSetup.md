1) Compile project
# MinGW :
 - cmake -B build -S . -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=ON -DCMAKE_PREFIX_PATH="c:/wxInstall" -DwxWidgets_LIB_DIR="C:/wxInstall/lib/gcc_x64_dll" -DwxLua_ROOT_DIR="c:/wxLua" -DBUILD_App=ON -DCOMPILE_SHADER=ON
 - cmake --build ./build -j8 --config Debug

# Visual Studio :

# GCC/make (Linux)