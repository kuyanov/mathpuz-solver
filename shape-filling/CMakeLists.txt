add_executable(shape-filling-sat shape-filling-sat.cpp)
add_dependencies(shape-filling-sat SatSolverBuild)

add_executable(shape-filling-x shape-filling-x.cpp)
target_link_libraries(shape-filling-x PRIVATE EvilThread)
