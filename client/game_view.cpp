#include "game_view.h"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


GameView::GameView():  
    sdl(SDL_INIT_VIDEO),
    window("CS2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
    renderer(window, -1, SDL_RENDERER_ACCELERATED),
    body_sprites(renderer, Surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp"))),
    legs_sprites(renderer, Surface(SDL_LoadBMP("../primer_fila_sin_padding.bmp"))),
    player_view(body_sprites),
    legs_view(legs_sprites,
                {
                    Rect(0,  0, 32, 32),   // Frame 1
                    Rect(32, 0, 32, 32),   // Frame 2
                    Rect(64, 0, 32, 32)    // Frame 3
                },
                100),
    background (renderer, Surface(SDL_LoadBMP("../dustroof.bmp"))){}


void GameView::update(const ActionDTO& action) {
    if (action.type == ActionType::MOVE) {
        float x = action.position[0];
        float y = action.position[1];
        legs_view.update_position(x, y);
        legs_view.update_animation();

        player_view.update_position(x, y);
    }
}

void GameView::render() {
    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.Clear();

    renderer.Copy(background, NullOpt, NullOpt);

    renderer.SetDrawColor(80, 80, 80, 255);
    renderer.FillRect(Rect(1, 1 ,1 , 1));
    renderer.FillRect(Rect(5 , 5 , 1, 2));
    renderer.FillRect(Rect(10 , 10 , 3, 3));
    renderer.FillRect(Rect(15 , 15 , 4, 8));

    legs_view.draw(renderer);
    player_view.draw(renderer);
    
    renderer.Present();

    SDL_Delay(1);  // Delay to limit frame rate (60 FPS)
}