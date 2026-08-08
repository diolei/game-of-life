#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

// Grid dimensions in cells; the window is COLS x ROWS at BLOCK_SIZE px per cell.
#define ROWS 120
#define COLS 240
#define BLOCK_SIZE 5

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} State;

// Grids, zero-initialized.
static unsigned char grid[ROWS][COLS];
static unsigned char next_grid[ROWS][COLS];

// Turn a cell on.
static void populate(int row, int col) {
    grid[row][col] = 1;
}

// Return 1 when the cell is alive; cells outside the grid are dead.
static int alive(int row, int col) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        return grid[row][col];
    }
    return 0;
}

// Count live neighboring cells.
static int live_neighbors(int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            count += alive(row + i, col + j);
        }
    }
    return count;
}

// Advance the simulation by one generation.
static void step(void) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int n = live_neighbors(row, col);
            if (grid[row][col]) {
                next_grid[row][col] = (n == 2 || n == 3);
            } else {
                next_grid[row][col] = (n == 3);
            }
        }
    }
    memcpy(grid, next_grid, sizeof(grid));
}

// Render the grid.
static void draw(State *state) {
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    // Draw live cells only; the cleared background is already black.
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (grid[row][col]) {
                SDL_Rect rect = {col * BLOCK_SIZE, row * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1};
                SDL_RenderFillRect(state->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(state->renderer);
}

// Initialize SDL; return 0 on failure.
static int init_sdl(State *state) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    state->window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            COLS * BLOCK_SIZE, ROWS * BLOCK_SIZE, SDL_WINDOW_SHOWN);

    if (state->window == NULL) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
    if (state->renderer == NULL) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(state->window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

// Release SDL resources.
static void quit_sdl(State *state) {
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    SDL_Quit();
}

int main(void) {
    State state;

    // Seed an R-pentomino: a five-cell pattern that evolves over 1,103
    // generations into a chaotic spread of gliders and oscillators.
    static const int pattern[][2] = {{0, 1}, {0, 2}, {1, 0}, {1, 1}, {2, 1}};
    for (size_t i = 0; i < sizeof pattern / sizeof pattern[0]; i++) {
        populate(15 + pattern[i][0], 110 + pattern[i][1]);
    }

    if (!init_sdl(&state)) {
        return 1;
    }

    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        draw(&state);
        step();

        SDL_Delay(16); // ~60 generations per second
    }

    quit_sdl(&state);
    return 0;
}
