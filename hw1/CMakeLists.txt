cmake_minimum_required(VERSION 3.6)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    README.md parser.cc parser.h raytra.cc)

add_executable(raytra ${SOURCE_FILES})