#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define ROW 600
#define COL 1200
#define BLOCK_SIZE 5

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} State;

// Uninitialized maps
int map[ROW][COL] = {0};
int new_map[ROW][COL] = {0};

// Turn cell on
void populateCells(int row, int col) {
    map[row][col] = 1;
}

// Check if cell is alive
int alive(int row, int col) {
    if (row >= 0 && row < ROW && col >= 0 && col < COL) {
        return map[row][col];
    }
    return 0;
}

// Check alive neighboring cells
int aliveNumber(int row, int col) {
    int cnt = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;
            if (alive(row + i, col + j)) {
                cnt++;
            }
        }
    }

    return cnt;
}

// Apply game logic
void logic(void) {
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            int alive_neighbors = aliveNumber(row, col);
            if (alive(row, col)) {
                if (alive_neighbors < 2) {
                    new_map[row][col] = 0; 
                } else if (alive_neighbors == 2 || alive_neighbors == 3) {
                    new_map[row][col] = 1;
                } else {
                    new_map[row][col] = 0;
                }
            } else {
                if (alive_neighbors == 3) {
                    new_map[row][col] = 1;
                }
            }
        }
    }

    // Copy maps
    memcpy(map, new_map, sizeof(map));
}

// Render screen
void drawScreen(State* state) {
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            if (map[row][col] == 1) {
                SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
            }

            SDL_Rect rect = {col * BLOCK_SIZE, row * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1};
            SDL_RenderFillRect(state->renderer, &rect);
        }
    }

    SDL_RenderPresent(state->renderer);
}

// SDL initialization
int initSDL(State* state) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    state->window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            COL, ROW, SDL_WINDOW_SHOWN);

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

// SDL cleanup
void quitSDL(State* state) {
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    SDL_Quit();
}

int main(void) {
    State state;

    // Populate cells
    populateCells(2, 1);
    populateCells(3, 2);
    populateCells(1, 3);
    populateCells(2, 3);
    populateCells(3, 3);

    if (!initSDL(&state)) {
        return 1;
    }

    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Activate screen
        drawScreen(&state);

        // Update state
        logic();

        // Adjust frame rate
        SDL_Delay(1);
    }

    quitSDL(&state);

    return 0;
}
