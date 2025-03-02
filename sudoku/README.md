# sudoku

There are two solvers available: `sudoku-sat` and `sudoku-x`, based on the SAT solver and the Knuth's algorithm X.
Both have the same interface:
```
../cmake-build-release/sudoku/sudoku-* < PUZZLE
```
where `PUZZLE` is the puzzle filename, such as `puzzles/9x9/1`.