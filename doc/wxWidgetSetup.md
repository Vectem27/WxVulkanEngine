# For windows :

1) Clone wxWidget 3.2.* or download from source

2) Compile wxWidgets
# With MSYS
 - cd path/to/wxWidgets
 - cmake -S . -B CMakeBuild -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/wxInstall
 - cmake --build CMakeBuild -j8
 - cmake --install CMakeBuild

# With Visual Studio
 - cmake . -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX="C:/wxInstall"
              "Visual Studio 16 2019"
              "Visual Studio 15 2017"
              "Visual Studio 14 2015"

Then you have to compile from the Visual Studio solution


# For Linux :
1) Install dev package dependancies : 
 - sudo apt-get install build-essential libgtk-3-dev libglib2.0-dev libwxgtk3.0-dev

2) Clone wxWidget 3.2.* or download from source

3) Compile wxWidgets
 - cd path/to/wxWidgets
 - ./configure --prefix=/usr/local --enable-unicode
 - make
 - make install