#include <cmath>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/action_DTO.h"

#include "input_handler.h"

using namespace SDL2pp;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define PLAYER_SPEED 0.2f
#define NUM_FRAMES 6  // 6 cuadros de animación

int main(int argc, char* argv[]) {
    try {
        // Inicialización
        SDL sdl(SDL_INIT_VIDEO);
        Window window("Jugador compuesto", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        // Cargar recursos
        Surface cuerpo_surface("assets/gfx/player/ct1.bmp");
        Texture cuerpo(renderer, cuerpo_surface.SetColorKey(true, 0));

        Surface piernas_surface("assets/gfx/player/legs.bmp");
        Texture piernas(renderer, piernas_surface);

        Surface mira_surface("assets/gfx/pointer.bmp");
        mira_surface.SetColorKey(true, SDL_MapRGB(mira_surface.Get()->format, 255, 0, 255));
        Texture mira(renderer, mira_surface);

        float posX = SCREEN_WIDTH / 2.0f - PLAYER_WIDTH / 2.0f;
        float posY = SCREEN_HEIGHT / 2.0f - PLAYER_HEIGHT / 2.0f;

        unsigned int prev_ticks = SDL_GetTicks();
        InputHandler ih(renderer, cuerpo, piernas);
        SDL_ShowCursor(SDL_DISABLE);
        bool running = true;

        // LoopClient
        while (running) {
            unsigned int frame_ticks = SDL_GetTicks();
            unsigned int frame_delta = frame_ticks - prev_ticks;
            prev_ticks = frame_ticks;

            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT ||
                    (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                    running = false;
                }
            }

            // Movimiento
            ActionDTO action_update = ih.receive_and_parse_action();

            ih.update_graphics(action_update);
        }

        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
