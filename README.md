# Conway's Game of Life

This is a simple implementation of Conway's Game of Life in C using the SDL library. 
Conway's Game of Life is a cellular automaton that simulates the evolution of a grid of cells over time. 
The game consists of a grid of cells, each of which can be in one of two states: alive or dead. The cells evolve from one generation to the next according to a set of rules.

## The Rules

The rules of Conway's Game of Life are straightforward:
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

## Getting Started

To run the simulation follow these steps: 

### Prerequisites

Before compiling and running this code, make sure you have the `SDL` library installed, you can install it on Debian-based distributions by running the following command:

```
apt install libsdl2-dev
```

### Compilation

To compile the code, make sure you have `make` installed. In the source directory run the following command:

```
make
```

### Usage

Run the `game` executable after compilation:

```
./game
```

### Cleanup

To remove the generated file, you can simply use the following command:

```
make clean
```
