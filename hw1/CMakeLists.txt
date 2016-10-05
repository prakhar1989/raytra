cmake_minimum_required(VERSION 3.6)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I. -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf")

set(SOURCE_FILES
    README.md parser.cc parser.h raytra.cc surface.h surface.cc camera.cc camera.h Material.cc Material.h ray.h exr.cc exr.h)

add_executable(raytra ${SOURCE_FILES})