#!/bin/bash

CMAKE_FLAGS='-D GLFW_USE_WAYLAND=1 -D BUILD_SHARED_LIBS=OFF -D GLFW_BUILD_EXAMPLES=OFF -D GLFW_BUILD_TESTS=OFF -D GLFW_BUILD_DOCS=OFF'

git submodule update build/submodules/glfw
modules_to_be_installed=''
all_modules="libwayland-dev libxkbcommon-dev wayland-protocols extra-cmake-modules"
for module in $all_modules; do
	dpkg -l | grep $module > /dev/null
	[ $? -ne 0 ] && modules_to_be_installed="${modules_to_be_installed}$module "
done
install_message="The password is only required to install certain packages which are essential for compiling GLFW.\nThe packages to be installed are:\n$modules_to_be_installed"
[[ -z $modules_to_be_installed ]] || (echo -e $install_message && sudo apt install -y $modules_to_be_installed)
cd build/submodules/glfw
[ -d build ] || mkdir build
cmake -S . -B build $CMAKE_FLAGS
cd build/
make
