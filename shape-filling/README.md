# shape-filling

There are two solvers available, `shape-filling-sat` and `shape-filling-x`, based on the SAT solver and the Knuth's algorithm X.
Both have the same interface:
```
../cmake-build-release/shape-filling/shape-filling-* LATTICE < PUZZLE
```
where `LATTICE` specifies the lattice used in the puzzle (such as `../lattices/square`), and `PUZZLE` is the puzzle filename (such as `puzzles/square_10x10_cornerless_septomino`).

To generate images, run
```
../cmake-build-release/shape-filling/shape-filling-* LATTICE < PUZZLE | python3 visualise.py LATTICE DESTINATION
```
where `DESTINATION` stands for the target directory for images.
