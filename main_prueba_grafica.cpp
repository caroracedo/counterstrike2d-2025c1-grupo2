#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const int PLAYER_SIZE = 50;
const int PLAYER_SPEED = 5;

int main(int argc, char* argv[]) {
    try {
        SDL sdl(SDL_INIT_VIDEO);

        Window window("prueba ventana", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);


        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        Rect player = {SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE / 2,
                       PLAYER_SIZE, PLAYER_SIZE};

        bool running = true;


        while (running) {

            SDL_Event event;
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

            renderer.SetDrawColor(255, 255, 255, 255);
            renderer.Clear();

            renderer.SetDrawColor(255, 0, 0, 255);
            renderer.FillRect(player);

            renderer.Present();

            SDL_Delay(16);
        }

        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
