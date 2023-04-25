#!/bin/bash

CMAKE_FLAGS='-D GLFW_USE_WAYLAND=1 -D BUILD_SHARED_LIBS=OFF -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF'

git submodule update build/submodules/glfw
sudo apt install libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules -y
cd build/submodules/glfw
[ -d build ] || mkdir build
cmake -S . -B build $CMAKE_FLAGS
cd build/
make
