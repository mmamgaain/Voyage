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
- [minimp3](https://github.com/lieff/minimp3 "minimp3") - Loading MP3 files (Still being implemented).

## Requirements
- g++
- Makefile

## Build Instructions
To run the only test in the project currently, simply navigate to the project directory and type `make` in the command line. This will build your entire project and also execute it. `make fresh` will remove all the intermediate object files and the executable.

`make compile_executable` and `make compile_debug_executable` will compile release and debug executables, respectively.
