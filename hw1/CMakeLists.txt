cmake_minimum_required(VERSION 3.6)
project(hw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    README.md parser.cc parser.h)

add_executable(hw1 ${SOURCE_FILES})