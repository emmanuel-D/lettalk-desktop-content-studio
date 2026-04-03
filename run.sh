#!/bin/bash

# Add Qt and CMake to the PATH (Linux)
export PATH="$HOME/Qt/Tools/CMake/bin:$HOME/Qt/6.11.0/gcc_64/bin:$PATH"

cmake -S . -B build && cmake --build build && ./build/LetTalkContentStudio
