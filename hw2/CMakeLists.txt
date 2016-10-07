cmake_minimum_required(VERSION 3.6)
project(hw2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I. -I/usr/include/OpenEXR -I/usr/local/include/OpenEXR -lIlmImf -lImath -lHalf -L/usr/local/lib")

set(SOURCE_FILES
    README.md parser.cc parser.h raytra.cc surface.h camera.cc camera.h Material.cc Material.h ray.h exr.cc exr.h sphere.cc sphere.h plane.cc plane.h)

add_executable(raytra ${SOURCE_FILES})