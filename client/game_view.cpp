#include "game_view.h"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


GameView::GameView():  
    sdl(SDL_INIT_VIDEO),
    window("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
    renderer(window, -1, SDL_RENDERER_ACCELERATED),
    legs_sprites(nullptr),
    body_sprites(nullptr),
    crosshair_sprites(nullptr) {
    load_sprites();
}

void GameView::run() try {

} catch(std::exception &e){

}

void GameView::load_leg_sprites() {
    legs_sprites = Texture(renderer, Surface("assets/gfx/player/legs.bmp"));
}

void GameView::load_body_postures() {
    body_sprites = Texture(renderer, Surface("assets/gfx/player/body.bmp"));
}

void GameView::load_crosshair_sprites() {
    crosshair_sprites = Texture(renderer, Surface("assets/gfx/pointer.bmp"));
}


// void render_player(SDL2pp::Renderer& renderer,
//             float x, float y,
//             float angle,
//             int frame = 0) {
// renderer.Copy(
//     body,
//     Rect(frame * width, 32, width, height),  // recorte del frame actual
//     Rect(static_cast<int>(x), static_cast<int>(y), width, height),  // destino en pantalla
//     angle,
//     SDL_Point{width / 2, height / 2},  // centro de rotación
//     SDL_FLIP_NONE
// );
// }