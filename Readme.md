# Voyage Rendering Engine

## Introduction
This is a **hobby project** that I've started in my spare time. This engine is being written in C++.
As this is a personal project, the scope of this project is very limited. **This project is only available on Linux as I work with Linux.**

This project contains libraries written by people more talented than I am. These libraries are:
- [GLM](https://github.com/g-truc/glm "OpenGL Mathematics (GLM)") - Standard Maths library.
- [GLFW](https://raw.githubusercontent.com/glfw/glfw "GLFW") - Library for window management and keyboard and mouse input handling.
- [glad](https://github.com/Dav1dde/glad "glad") - Modern OpenGL library for C++.
- [stb](https://github.com/nothings/stb "stb") - Image loading.
- [ImGui](https://github.com/ocornut/imgui "Dear ImGui") - Immediate Mode GUI; absolutely amazing.
- [Assimp](https://github.com/assimp/assimp "Open Asset Import Library (assimp)") - Model loading.
- [OpenAL](https://github.com/kcat/openal-soft "OpenAL soft") - GPU accelerated sounds.
- [libsndfile](https://github.com/libsndfile/libsndfile "libsndfile") - Loading sound files (except MP3 files).
- [minimp3](https://github.com/lieff/minimp3 "minimp3") - Loading MP3 files.

## Requirements
- g++
- Makefile

## Basic Build Instructionsi (for running tests)
To run the only test in the project currently, simply navigate to the project directory and type `make` in the command line. This will build your entire project and also execute it. `make fresh` will remove all the intermediate object files and the executable.

`make compile_executable` and `make compile_debug_executable` will compile release and debug executables, respectively.

## Build Instructions
There are two main ways to build and work with this project:
### 1. Building as an external library:
`make compile_as_static_library` and `make compile_as_dynamic_library` are commands to compile the project as a static and dynamic library, respectively. The static library output "libvoyage.a" and the dynamic library output "libvoyage.so" are produced in the base directory. All the header files are available in `lib/` directory and all the binary files for external libraries are in the `build/precompiled/` directory.
### 2. Building with project skeleton (Recommended):
To execute this, issue command `make export`. This will create a directory with a default name of "Voyage_project" at the current location of this project. This project will contain all the directories and files already created. The only reason that this method is recommended is that it creates the whole structure and even creates a stock main file.
