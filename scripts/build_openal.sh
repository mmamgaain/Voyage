#!/bin/bash

git submodule update build/submodules/openal
cd build/submodules/openal/build/
cmake ..
cmake --build .

