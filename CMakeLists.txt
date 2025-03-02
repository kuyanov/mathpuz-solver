cmake_minimum_required(VERSION 3.5)
project(mathpuz_solver)

enable_language(C ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_ASAN "-g -fsanitize=address")

include(cmake/cadical.cmake)
include(cmake/kissat.cmake)

add_custom_target(SatSolverBuild DEPENDS
        "${CMAKE_BINARY_DIR}/cadical"
        "${CMAKE_BINARY_DIR}/kissat"
)
add_compile_definitions(BIN_DIR="${CMAKE_BINARY_DIR}")

include_directories(include include/evil)

file(GLOB EVIL_SRC include/evil/*.s include/evil/*.cpp)
add_library(EvilThread STATIC ${EVIL_SRC})

add_subdirectory(kropki)
add_subdirectory(shape-dividing)
add_subdirectory(shape-filling)
add_subdirectory(sudoku)
