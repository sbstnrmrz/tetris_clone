#ifndef GAME_H
#define GAME_H

#include "defs.h"

#define RED    (SDL_Color) {255, 0, 0, 255}
#define GREEN  (SDL_Color) {0, 255, 0, 255}
#define BLUE   (SDL_Color) {0, 0, 255, 255}
#define CYAN   (SDL_Color) {0, 255, 255, 255}
#define YELLOW (SDL_Color) {255, 255, 0, 255}
#define ORANGE (SDL_Color) {255, 80, 0, 255}
#define PURPLE (SDL_Color) {255, 0, 255, 255}

#define ROWS 20
#define COLS 10

typedef enum {
    I = 1,
    O = 2,
    T = 3,
    S = 4,
    Z = 5,
    J = 6,
    L = 7
} shapeName;

typedef struct {
    int x[4];
    int y[4];
    shapeName name;
    SDL_Color color;
} Tetromino;

void reset_board();
void input(SDL_Event event);
Tetromino gen_mino();
void fill_bag();
bool check_place_mino();
void check_mino_colission();
void place_mino();
int clear_lines();
void update_board();
void update_tetromino(u64 frames);
void render_board(SDL_Renderer *renderer);
void render_tetromino(SDL_Renderer *renderer);
void debug_board();
void debug_mino();
void debug_bag(); 

#endif // GAME_H
