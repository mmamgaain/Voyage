#!/bin/bash

git submodule update build/submodules/imgui
[ -d lib/imgui/ ] || mkdir lib/imgui/
cp --preserve=mode,ownership,timestamps -u -t lib/imgui/ build/submodules/imgui/*.h build/submodules/imgui/*.cpp build/submodules/imgui/backends/*_impl_glfw* build/submodules/imgui/backends/*_impl_opengl3*
