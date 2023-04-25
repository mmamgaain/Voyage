#!/bin/bash

git submodule update build/submodules/stb
cd build/submodules/stb/
[ -f stb_image.cpp ] || echo -e "#define STB_IMAGE_IMPLEMENTATION\n\n#include \"stb/stb_image.h\"\n" > stb_image.cpp
[ -f stb_perlin.cpp ] || echo -e "#define STB_PERLIN_IMPLEMENTATION\n\n#include \"stb/stb_perlin.h\"\n" > stb_perlin.cpp
