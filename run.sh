#!/bin/bash

# Add Qt and CMake to the PATH
if [[ "$(uname)" == "Darwin" ]]; then
  export PATH="$HOME/Qt/Tools/CMake/CMake.app/Contents/bin:$HOME/Qt/6.9.1/macos/bin:$PATH"
else
  export PATH="$HOME/Qt/Tools/CMake/bin:$HOME/Qt/6.11.0/gcc_64/bin:$PATH"
fi

cmake -S . -B build && cmake --build build

# Run the executable (macOS uses an app bundle)
if [[ -d ./build/LetTalkContentStudio.app ]]; then
  ./build/LetTalkContentStudio.app/Contents/MacOS/LetTalkContentStudio
else
  ./build/LetTalkContentStudio
fi
