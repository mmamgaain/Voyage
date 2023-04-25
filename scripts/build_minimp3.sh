#!/bin/bash

git submodule update build/submodules/minimp3
cd build/submodules/minimp3
echo -e "#define MINIMP3_IMPLEMENTATION\n#define MINIMP3_ONLY_SIMD\n#include \"minimp3.h\"\n#include \"minimp3_ex.h\"" > minimp3.cpp
