# Conway's Game of Life

Conway's Game of Life is a cellular automaton that simulates the evolution of a grid of cells over time. Each cell is alive or dead, and every generation follows the same four rules. The simulation starts with an R-pentomino, a five-cell pattern that evolves over 1,103 generations into a spread of gliders and oscillators.

## Rules

1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
2. Any live cell with two or three live neighbors lives on to the next generation.
3. Any live cell with more than three live neighbors dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

Cells outside the grid count as dead.

## Prerequisites

Install SDL2; on Debian-based distributions:

```
apt install libsdl2-dev
```

## Build

```
make
```

Remove the binary with `make clean`.

## Usage

```
./game
```

Close the window to quit.
