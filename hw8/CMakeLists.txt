cmake_minimum_required(VERSION 3.6)
project(hw8)
set(CMAKE_CXX_STANDARD 11)

set(GLFW_SOURCE_DIR "/usr/local/include/GLFW")
include_directories(/usr/local/include)
include_directories(${GLFW_SOURCE_DIR}/deps)
set(GL_LIBS glfw3.a)
link_directories(/usr/local/lib)

IF(APPLE)
    find_library(COCOA Cocoa)
    find_library(OPEN_GL OpenGL)
    find_library(IOKIT IOKit)
    find_library(COREVIDEO CoreVideo)
    set(FRAMEWORKS ${COCOA} ${OPEN_GL} ${IOKIT} ${COREVIDEO})
ENDIF()

set(GLAD "${GLFW_SOURCE_DIR}/deps/glad.c")
set(SOURCE_FILES utils.h parser.cc initshader.cc glrender.cc)

add_executable(glrender ${GLAD} ${SOURCE_FILES})
target_link_libraries(glrender ${FRAMEWORKS} ${GL_LIBS} pthread dl)
