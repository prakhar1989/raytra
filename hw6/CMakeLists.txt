cmake_minimum_required(VERSION 3.6)
project(hw5)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/include)
include_directories(/usr/local/include/OpenEXR)

set(EXR_LIBS IlmImf.a Imath.a Half.a)
link_directories(/usr/local/lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
file(GLOB SOURCE_FILES  "*.cc")
add_executable(hw5 ${SOURCE_FILES})
target_link_libraries(hw5 ${EXR_LIBS})

set(TESTABLE_SOURCES triangle.h triangle.cc bounding_box.cc BVHTree.cc)
file(GLOB TEST_FILES "tests/*.cc")
add_executable(hw5_test ${TEST_FILES} ${TESTABLE_SOURCES})
