cmake_minimum_required(VERSION 3.16)
project(chess CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable with all source files
add_library(chess_lib STATIC
    src/core/board.cpp
    src/core/move.cpp
    src/core/moveGen.cpp
    src/engine/engine.cpp
    src/engine/piece_tables.cpp
)
target_include_directories(chess_lib PUBLIC
    src/
    src/core/
    src/engine/
)

add_executable(chess src/ChessGameLoop.cpp)
target_link_libraries(chess PRIVATE chess_lib)

#Catch2 for unit tests
find_package(Catch2 3 REQUIRED)
add_executable(tests
    tests/unit_tests/board_setup.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain chess_lib)
target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)