cmake_minimum_required (VERSION 2.6)
project (Perlin)
add_executable (Perlin main.cpp module.hpp module.cpp modules/rand2d.hpp modules/world.hpp modules/world.cpp chunk.hpp chunk.cpp noise2d.hpp noise2d.cpp modules/noise3d.cpp modules/noise3d.hpp)
target_link_libraries(Perlin sfml-window sfml-system sfml-graphics)
