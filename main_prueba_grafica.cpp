#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const int PLAYER_SIZE = 50;
const int PLAYER_SPEED = 5;


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    // IMG_Init(IMG_INIT_PNG);

    // if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    //     std::cerr << "Error al inicializar SDL_image: " << IMG_GetError() << std::endl;
    //     SDL_Quit();
    //     return 1;
    // }

    SDL_Window* window =
            SDL_CreateWindow("prueba ventana", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    // SDL_Texture* background = IMG_LoadTexture(renderer, "assets/gfx/sprites/aztecroof.png");
    // if (!background) {
    //     std::cerr << "No se pudo cargar 'mapa.png': " << IMG_GetError() << std::endl;
    //     return 1;
    // }

    SDL_Rect player = {SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2,
                       PLAYER_SIZE, PLAYER_SIZE};

    bool running = true;

    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_W])
            player.y -= PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_S])
            player.y += PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_A])
            player.x -= PLAYER_SPEED;
        if (keystates[SDL_SCANCODE_D])
            player.x += PLAYER_SPEED;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    // SDL_DestroyTexture(background);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
