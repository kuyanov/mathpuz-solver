# shape-dividing

There is one solver currently available: `shape-dividing-sat`, based on the SAT solver.
Run it by executing
```
../cmake-build-release/shape-dividing/shape-dividing-sat LATTICE < PUZZLE
```
where `LATTICE` specifies the lattice used in the puzzle (such as `../lattices/triangular_median`), and `PUZZLE` is the puzzle filename (such as `puzzles/triangle_5x5_median/5p`).

To generate images, run
```
../cmake-build-release/shape-dividing/shape-dividing-sat LATTICE < PUZZLE | python3 visualise.py LATTICE DESTINATION
```
where `DESTINATION` stands for the target directory for images.
