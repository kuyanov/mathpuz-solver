# shape-dividing

Execute
```
../cmake-build-release/shape-dividing/shape-dividing-sat LATTICE < PUZZLE
```
where `LATTICE` specifies the lattice of the figure (such as `../lattices/triangular_median`), and `PUZZLE` specifies the figure filename (such as `puzzles/triangle_5x5_median/5p`).

To generate images, run
```
../cmake-build-release/shape-dividing/shape-dividing-sat LATTICE < PUZZLE | python3 visualise.py LATTICE DESTINATION
```
where `DESTINATION` specifies the output directory for images.
