#!/bin/bash

g++ /home/cs4160/glfw/deps/glad.c initshader.cc glrender.cc -I/home/cs4160/glfw/include/ -I/home/cs4160/glfw/deps -L/usr/lib/x86_64-linux-gnu/ -L/home/cs4160/glfw/lib/ -Wl,--no-as-needed -lglfw3 -lGLU -lGL -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm -std=c++11 -o glrender
