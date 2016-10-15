cmake_minimum_required(VERSION 3.6)
project(hw3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I. -I/usr/include/OpenEXR -I/usr/local/include/OpenEXR -I/usr/local/include -lIlmImf -lImath -lHalf -L/usr/local/lib")

set(SOURCE_FILES
    camera.cc
    camera.h
    exr.cc
    exr.h
    Material.cc
    Material.h
    parser.cc
    parser.h
    plane.cc
    plane.h
    point_light.cc
    point_light.h
    ray.h
    raytra.cc
    raytra.h
    sphere.cc
    sphere.h
    surface.h)

add_executable(hw3 ${SOURCE_FILES})