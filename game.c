#include "game.h"
#include "defs.h"

/*
const Tetromino shapes[7] = {(Tetromino){{0, 1, 2, 3}, {2, 2, 2, 2}, I, CYAN},
                             (Tetromino){{0, 1, 2, 3}, {2, 2, 2, 2}, O, YELLOW},
                             (Tetromino){{1, 0, 1, 2}, {2, 3, 3, 3}, T, PURPLE},
                             (Tetromino){{1, 2, 0, 1}, {2, 2, 3, 3}, S, GREEN},
                             (Tetromino){{0, 1, 1, 2}, {2, 2, 3, 3}, Z, RED},
                             (Tetromino){{0, 0, 1, 2}, {2, 3, 3, 3}, J, BLUE},
                             (Tetromino){{2, 0, 1, 2}, {2, 3, 3, 3}, L, ORANGE}};
*/

const float level[30] = {800.0f,      // LEVEL 0  FRAMES 48 
                         716.666666f, // LEVEL 1  FRAMES 43
                         633.333333f, // LEVEL 2  FRAMES 38
                         550.0f,      // LEVEL 3  FRAMES 33
                         466.666666f, // LEVEL 4  FRAMES 28
                         383.333333f, // LEVEL 5  FRAMES 23
                         300.0f,      // LEVEL 6  FRAMES 18
                         216.666666f, // LEVEL 7  FRAMES 13
                         133.333333f, // LEVEL 8  FRAMES 8
                         100.0f,      // LEVEL 9  FRAMES 6
                         83.333333f,  // LEVEL 10 FRAMES 5
                         83.333333f,  // LEVEL 11 FRAMES 5
                         83.333333f,  // LEVEL 12 FRAMES 5
                         66.666666f,  // LEVEL 13 FRAMES 4
                         66.666666f,  // LEVEL 14 FRAMES 4
                         66.666666f,  // LEVEL 15 FRAMES 4
                         50.0f,       // LEVEL 16 FRAMES 3
                         50.0f,       // LEVEL 17 FRAMES 3
                         50.0f,       // LEVEL 18 FRAMES 3
                         33.333333f,  // LEVEL 19 FRAMES 2
                         33.333333f,  // LEVEL 20 FRAMES 2
                         33.333333f,  // LEVEL 21 FRAMES 2
                         33.333333f,  // LEVEL 22 FRAMES 2
                         33.333333f,  // LEVEL 23 FRAMES 2
                         33.333333f,  // LEVEL 24 FRAMES 2
                         33.333333f,  // LEVEL 25 FRAMES 2
                         33.333333f,  // LEVEL 26 FRAMES 2
                         33.333333f,  // LEVEL 27 FRAMES 2
                         33.333333f,  // LEVEL 28 FRAMES 2
                         16.666666f}; // LEVEL 29 FRAMES 1

const Tetromino shapes[7] = {(Tetromino){{2, 2, 2, 2}, {0+3, 1+3, 2+3, 3+3}, I, CYAN},
                             (Tetromino){{1, 1, 2, 2}, {1+3, 2+3, 1+3, 2+3}, O, YELLOW},
                             (Tetromino){{1, 2, 2, 2}, {1+3, 0+3, 1+3, 2+3}, T, PURPLE},
                             (Tetromino){{1, 1, 2, 2}, {1+3, 2+3, 0+3, 1+3}, S, GREEN},
                             (Tetromino){{1, 1, 2, 2}, {0+3, 1+3, 1+3, 2+3}, Z, RED},
                             (Tetromino){{1, 2, 2, 2}, {0+3, 0+3, 1+3, 2+3}, J, BLUE},
                             (Tetromino){{1, 2, 2, 2}, {2+3, 0+3, 1+3, 2+3}, L, ORANGE}};

char      charBoard[ROWS][COLS] = {0};
u8        intBoard[ROWS][COLS] = {0};
Tetromino mino = {0};
u8        bag[7] = {0};
size_t    bagPos = 0;
bool minoExist = false;
bool bagEmpty = true;

SDL_Rect board = {.x = WIN_WIDTH / 2 - (CELL_SIZE * 10) / 2, 
                  .y = WIN_HEIGHT / 2 - (CELL_SIZE * 20) / 2,
                  .w = CELL_SIZE * 10,
                  .h = CELL_SIZE * 20};

void reset_board() {
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLS; j++) {
            charBoard[i][j] = '.';
            intBoard[i][j] = 0;
        }
    }

}

Tetromino gen_mino() {
    if (bagEmpty) {
        size_t counter = 0;

        while (counter < 7) {
            bool repeat = false; 
            bag[counter] = rand() % 7;
            for (size_t i = 0; i < counter; i++) {
                if (bag[counter] == bag[i] && i != counter) {
                    repeat = true;
                }
            } 
            if (repeat) {
                continue;
            } else {
                counter++;
            }
        }
        bagEmpty = false;
    }

    return shapes[bag[bagPos]];
}

void check_mino_colission() {
    for (size_t i = 0; i < 4; i++) {
        if (mino.y[i] < 0) {
            for (size_t j = 0; j < 4; j++) {
                mino.y[j]++;
            }
        }
        if (mino.y[i] > COLS-1) {
            for (size_t j = 0; j < 4; j++) {
                mino.y[j]--;
            }
        }
    }

}

bool check_place_mino() {
    bool result = false;
    for (size_t i = 0; i < 4; i++) {
        if (intBoard[mino.x[i]+1][mino.y[i]] > 0 || mino.x[i] >= ROWS-1) {
            result = true;
        }
    }

    return result;
}

void place_mino() {
    for (size_t i = 0; i < 4; i++) {
        intBoard[mino.x[i]][mino.y[i]] = mino.name;
    }

    bagPos++;
    minoExist = false;
}

int clear_lines() {
    int line = 0;

    for (size_t i = ROWS; i >= 0; i--) {
        if (line >= 40) {
            break;
        }

        for (size_t j = 0; j < COLS; j++) {
            if (intBoard[i][j] > 0) {
                line++; 
            } else {
                line -= j;
                break;
            }
        }
        
        if (line % 10 == 0) {
            for (size_t k = 0; k < COLS; k++) {
                intBoard[i][k] = 0;
            }
            
            for (size_t l = i; l > 0; l--) {
                for (size_t k = 0; k < COLS; k++) {
                    intBoard[l][k] = intBoard[l-1][k];
                }
            }
        }
    }

    return 0;
}

void input(SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_UP) {
            
        }
        if (event.key.keysym.sym == SDLK_DOWN) {

        }
        if (event.key.keysym.sym == SDLK_LEFT) {
            for (size_t i = 0; i < 4; i++) {
                mino.y[i]--;
            }        
        }
        if (event.key.keysym.sym == SDLK_RIGHT) {
            for (size_t i = 0; i < 4; i++) {
                mino.y[i]++;
            }        
        }
    }

}

void update_board() {



}

void render_board(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &board);

    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLS; j++) {

            if (intBoard[i][j] > 0) {
                for (size_t k = 0; k < 7; k++) {
                    if (intBoard[i][j] == shapes[k].name) {
                        SDL_SetRenderDrawColor(renderer, 
                                               shapes[k].color.r, 
                                               shapes[k].color.g, 
                                               shapes[k].color.b, 
                                               shapes[k].color.a);
                    }
                }

                SDL_Rect cell = {.x = board.x + j * CELL_SIZE, 
                                 .y = board.y + i * CELL_SIZE,
                                 .w = CELL_SIZE,
                                 .h = CELL_SIZE};

                SDL_RenderFillRect(renderer, &cell);
                SDL_RenderDrawRect(renderer, &cell);
            }
        }
    }

}

void update_tetromino(u64 frames) {
    if (bagPos >= 7) {
        bagPos = 0;
        bagEmpty = true;
    }

    if (!minoExist) {
        mino = gen_mino();
        minoExist = true;
    }

    check_mino_colission();
    
    if (frames % 23 == 0) {
        if (check_place_mino()) {
            place_mino();
        } else {
            for (size_t i = 0; i < 4; i++) {
                mino.x[i]++;        
            }
        }
    }
        
}

void render_tetromino(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 
                           mino.color.r, 
                           mino.color.g, 
                           mino.color.b, 
                           mino.color.a);
    
    for (size_t i = 0; i < 4; i++) {
        SDL_Rect cell = {.x = mino.y[i] * CELL_SIZE + board.x,
                         .y = mino.x[i] * CELL_SIZE + board.y,
                         .w = CELL_SIZE,
                         .h = CELL_SIZE};

        SDL_RenderFillRect(renderer, &cell);
        SDL_RenderDrawRect(renderer, &cell);
    }

}

void debug_board() {
    printf("[BOARD INFO]\n\n");

    for (size_t i = 0; i < ROWS; i++) {
        printf("|");
        for (size_t j = 0; j < COLS; j++) {
            printf("%d|", intBoard[i][j]);    
        }
/*
        printf("\t|");
        for (size_t k = 0; k < COLS; k++) {
            printf("%d|", intBoard[i][k]);
        }
*/
        printf("\n");
    }

}

void debug_mino() {
    printf("[TETROMINO INFO]\n\n"); 
    printf("X : Y\n");

    for (size_t i = 0; i < 4; i++) {
        printf("%d, %d\n", mino.x[i], mino.y[i]);
    }

}

void debug_bag() {
    printf("[BAG INFO]\n");

    for (size_t i = 0; i < 7; i++) {
        printf("BAG: %d\n", bag[i]);    
    }
    printf("BAGPOS: %zu\n", bagPos);

}
