#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <cmath>

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
        InputHandler ih;
        SDL_ShowCursor(SDL_DISABLE);
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

            // Movimiento
            ActionDTO action = ih.receive_and_parse_action();
            float delta = PLAYER_SPEED * frame_delta;

            bool moving = false;
            if (action.type == ActionType::MOVE) {
                moving = true;
                switch (action.direction) {
                    case Direction::UP:    posY -= delta; break;
                    case Direction::DOWN:  posY += delta; break;
                    case Direction::LEFT:  posX -= delta; break;
                    case Direction::RIGHT: posX += delta; break;
                    default: break;
                }
            }

            // Centro del jugador
            float centerX = posX + PLAYER_WIDTH / 2.0f;
            float centerY = posY + PLAYER_HEIGHT / 2.0f;

            // Mouse
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            float dx = mouseX - centerX;
            float dy = mouseY - centerY;
            float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

            // Frame de animación
            static int anim_frame = 0;
            static int anim_timer = 0;
            anim_timer += frame_delta;
            if (moving && anim_timer > 100) {
                anim_frame = (anim_frame + 1) % NUM_FRAMES;
                anim_timer = 0;
            }

            // Calcular fila y columna del frame
            int col = anim_frame % 3;
            int row = anim_frame / 3;

            int frame_w = 42;  // aproximado según imagen
            int frame_h = 32;

            Rect src_legs(col * frame_w, row * frame_h, frame_w, frame_h);

            // Render
            renderer.SetDrawColor(255, 255, 255, 255);
            renderer.Clear();

            // Piernas (sin rotar)
            renderer.Copy(
                piernas,
                src_legs,
                Rect(static_cast<int>(posX), static_cast<int>(posY), PLAYER_WIDTH, PLAYER_HEIGHT)
            );

            // Cuerpo rotado
            renderer.Copy(
                cuerpo,
                Rect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT),
                Rect(static_cast<int>(posX), static_cast<int>(posY), PLAYER_WIDTH, PLAYER_HEIGHT),
                angle,
                SDL_Point{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2},
                SDL_FLIP_NONE
            );

            // Mira
            int mira_w = 46, mira_h = 46;
            renderer.Copy(
                mira,
                Rect(0, 0, mira_w, mira_h),
                Rect(mouseX - mira_w / 2, mouseY - mira_h / 2, mira_w, mira_h)
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
