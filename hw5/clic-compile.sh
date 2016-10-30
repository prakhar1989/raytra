#!/bin/bash
g++ -O3 *.cc -I. -I/usr/include/OpenEXR -lIlmImf -lImath -lHalf -Wall -std=c++11 -o prog_out

