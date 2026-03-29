#!/bin/bash

export PATH="$HOME/Qt/Tools/CMake/CMake.app/Contents/bin:$HOME/Qt/6.9.1/macos/bin:$PATH"

qt-cmake -S . -B build && cmake --build build && ./build/LetTalkContentStudio.app/Contents/MacOS/LetTalkContentStudio
