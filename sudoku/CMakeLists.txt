add_executable(sudoku-sat sudoku-sat.cpp)
add_dependencies(sudoku-sat SatSolverBuild)

add_executable(sudoku-x sudoku-x.cpp)
