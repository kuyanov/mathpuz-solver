add_executable(sudoku-sat sudoku-sat.cpp)
add_dependencies(sudoku-sat SatSolverBuild)

add_executable(sudoku-x sudoku-x.cpp)
target_link_libraries(sudoku-x PRIVATE EvilThread)

add_executable(sudoku-gen sudoku-gen.cpp)
