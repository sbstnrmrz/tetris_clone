#include "defs.h"
#include "game.h"

struct {
    u64 frames;
    bool running;   
    SDL_Window *window;
    SDL_Renderer *renderer;
} engine;

void init_sdl(const char *title, int winxPos, int winyPos, int winWidth, int winHeight, bool fullscreen) {
    engine.running = false;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Failed to initialize SDL. SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    engine.window = SDL_CreateWindow(title, winxPos, winyPos, winWidth, winHeight, fullscreen);
    if (engine.window == NULL) {
        fprintf(stderr, "Failed to create SDL_Window. SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (engine.renderer == NULL) {
        fprintf(stderr, "Failed to create SDL_Renderer. SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
 
    engine.running = true;
}

void handle_events() {
    SDL_Event event;
    SDL_PollEvent(&event);

    if (event.type == SDL_QUIT) {
        engine.running = false;
    }

    input(event);

}

void update() {
    update_board();
    update_tetromino(engine.frames);

    engine.frames++;
//    printf("%llu\n", SDL_GetTicks64());  
}

void render() {
    SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 255);
    SDL_RenderClear(engine.renderer);

    render_board(engine.renderer);
    render_tetromino(engine.renderer);

    SDL_RenderPresent(engine.renderer);
    
}

void debug() {
//    debug_board();
//    printf("\n");
//    debug_mino();

}

void clean() {
    SDL_DestroyRenderer(engine.renderer);
    SDL_DestroyWindow(engine.window);

    SDL_Quit();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    reset_board();

    init_sdl("TETRIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, false);

    while(engine.running) {
        handle_events();
        update();
        render();
        debug(); 
    }

    clean();

    return 0;
}
