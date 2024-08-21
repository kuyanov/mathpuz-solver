cmake_minimum_required(VERSION 3.5)
project(mathpuz_solver)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address")

include(cmake/cadical.cmake)
include(cmake/kissat.cmake)

add_custom_target(SatSolverBuild DEPENDS
        "${CMAKE_BINARY_DIR}/cadical"
        "${CMAKE_BINARY_DIR}/kissat"
)
add_compile_definitions(BIN_DIR="${CMAKE_BINARY_DIR}")

include_directories(include)

add_subdirectory(kropki)
add_subdirectory(shape-cutting)
add_subdirectory(shape-filling)
add_subdirectory(sudoku)
