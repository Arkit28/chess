cmake_minimum_required(VERSION 3.16)
project(chess CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable with all source files
add_executable(chess
    src/ChessGameLoop.cpp
    src/core/board.cpp
    src/core/move.cpp
    src/core/moveGen.cpp
    src/engine/engine.cpp
)

# Include directories for headers
target_include_directories(chess PRIVATE
    src/
    src/core/
    src/engine/
)