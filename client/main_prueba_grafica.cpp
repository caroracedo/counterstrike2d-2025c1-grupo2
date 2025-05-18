#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <cmath>

#include "../common/action_DTO.h"
#include "input_handler.h"
#include "player_view.h"


using namespace SDL2pp;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PLAYER_WIDTH 32  
#define PLAYER_HEIGHT 32
#define PLAYER_SPEED 0.2f

int main(int argc, char* argv[]) {
    try {
        SDL sdl(SDL_INIT_VIDEO);

        Window window("Jugador compuesto", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        Surface cuerpo_surface("assets/gfx/player/ct1.bmp");
        Texture cuerpo(renderer, cuerpo_surface.SetColorKey(true, 0));
        
        PlayerView player_view(cuerpo);

        float posX = SCREEN_WIDTH / 2.0f - PLAYER_WIDTH / 2.0f;
        float posY = SCREEN_HEIGHT / 2.0f - PLAYER_HEIGHT / 2.0f;

        Surface mira_surface = Surface("assets/gfx/pointer.bmp");

        mira_surface.SetColorKey(true, SDL_MapRGB(mira_surface.Get()->format, 255, 0, 255));

        Texture mira(renderer, mira_surface);

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
            
            // Obtener acción del jugador
            ActionDTO action = ih.receive_and_parse_action();
            float delta = PLAYER_SPEED * frame_delta;

            if (action.type == ActionType::MOVE) {
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

            // Posición del mouse
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Calcular ángulo hacia el mouse (ajustado si el sprite mira hacia arriba)
            float dx = mouseX - centerX;
            float dy = mouseY - centerY;
            float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

            // Dibujar fondo
            renderer.SetDrawColor(255, 255, 255, 255);
            renderer.Clear();

            player_view.render(renderer,posX,posY,angle);

            int mira_w = 46, mira_h = 46;

            Rect src_cursor_rect(0, 0, mira_w, mira_h);

            renderer.Copy(
                mira,
                src_cursor_rect,
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
