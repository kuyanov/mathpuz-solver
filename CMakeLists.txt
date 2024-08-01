cmake_minimum_required(VERSION 3.5)
project(mathpuz_solver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address")

include(cmake/kissat.cmake)

include_directories(include)

add_subdirectory(kropki)
add_subdirectory(sudoku)
