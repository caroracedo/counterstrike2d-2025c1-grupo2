#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50
#define PLAYER_SPEED 0.2f
#define NUM_FRAMES 8

int main(int argc, char* argv[]) {
    try {
        SDL sdl(SDL_INIT_VIDEO);

        Window window("Jugador compuesto", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        
        Surface cuerpo_surface("assets/gfx/player/ct1.bmp");

    

        Texture cuerpo(renderer, cuerpo_surface.SetColorKey(true, 0));

        Texture piernas(renderer, "assets/gfx/player/legs.bmp");

        float posX = SCREEN_WIDTH / 2.0f - PLAYER_WIDTH / 2.0f;
        float posY = SCREEN_HEIGHT / 2.0f - PLAYER_HEIGHT / 2.0f;

        bool moving = false;
        int run_phase = 0;

        unsigned int prev_ticks = SDL_GetTicks();

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

            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            moving = false;

            if (keystates[SDL_SCANCODE_W]) { posY -= PLAYER_SPEED * frame_delta; moving = true; }
            if (keystates[SDL_SCANCODE_S]) { posY += PLAYER_SPEED * frame_delta; moving = true; }
            if (keystates[SDL_SCANCODE_A]) { posX -= PLAYER_SPEED * frame_delta; moving = true; }
            if (keystates[SDL_SCANCODE_D]) { posX += PLAYER_SPEED * frame_delta; moving = true; }

            // Limitar dentro de pantalla
            if (posX < 0) posX = 0;
            if (posY < 0) posY = 0;
            if (posX + PLAYER_WIDTH > SCREEN_WIDTH) posX = SCREEN_WIDTH - PLAYER_WIDTH;
            if (posY + PLAYER_HEIGHT > SCREEN_HEIGHT) posY = SCREEN_HEIGHT - PLAYER_HEIGHT;

            // Animación de piernas
            run_phase = moving ? (frame_ticks / 100) % NUM_FRAMES : 0;

            // Render
            renderer.SetDrawColor(255, 255, 255, 255);
            renderer.Clear();

            // Dibujar piernas animadas
            renderer.Copy(
                piernas,
                Rect(run_phase * 32, 0, 32, 64), // recorte del frame de animación
                Rect(static_cast<int>(posX), static_cast<int>(posY + 32 / 2), 32, 64)
            );

            // Dibujar cuerpo por encima
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
