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
// #define NUM_FRAMES 6


int main(int argc, char* argv[]) {
    try {
        SDL sdl(SDL_INIT_VIDEO);

        Window window("Jugador compuesto", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        Surface cuerpo_surface("assets/gfx/player/ct1.bmp");

        Texture cuerpo(renderer, cuerpo_surface.SetColorKey(true, 0));

        float posX = SCREEN_WIDTH / 2.0f - PLAYER_WIDTH / 2.0f;
        float posY = SCREEN_HEIGHT / 2.0f - PLAYER_HEIGHT / 2.0f;

        // bool moving = false;
        // int run_phase = 0;

        unsigned int prev_ticks = SDL_GetTicks();
        InputHandler ih;
        bool running = true;
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
            
            // Obtener acción del jugador
            ActionDTO action = ih.receive_and_parse_action();

            // moving = false;
            float delta = PLAYER_SPEED * frame_delta;

            if (action.type == ActionType::MOVE) {
                // moving = true;
                switch (action.direction) {
                    case Direction::UP:    posY -= delta; break;
                    case Direction::DOWN:  posY += delta; break;
                    case Direction::LEFT:  posX -= delta; break;
                    case Direction::RIGHT: posX += delta; break;
                    default: break;
                }
            }   
            // if (moving) {
            //     run_phase = (frame_ticks / 100) % NUM_FRAMES;
            // } else {
            //     run_phase = 0;
            // }              
            renderer.SetDrawColor(255, 255, 255, 255);
            renderer.Clear();

            renderer.Copy(
                cuerpo,
                Rect(0, 32, 32, 32),  // Solo esta región de la imagen
                Rect(static_cast<int>(posX), static_cast<int>(posY), 32, 32)  // misma posición que las piernas
            );

            renderer.Present();
            SDL_Delay(1);
        }

        return 0;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
