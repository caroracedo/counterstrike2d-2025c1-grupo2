#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

class GameView {
private:
    SDL sdl;
    Window window;
    Renderer renderer;

    Texture legs_sprites;
    Texture body_sprites;
    Texture crosshair_sprites;

public:
    GameView();
    void run();
};

#endif
