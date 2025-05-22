#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "player_view.h"
#include "../common/action_DTO.h"
#include "legs_view.h"
#include "game_camera.h"

using namespace SDL2pp;

class GameView {
private:
    SDL sdl;
    Window window;
    Renderer renderer;

    Texture body_sprites;
    Texture legs_sprites;   
    
    PlayerView player_view; 
    LegsView legs_view;

    Texture background;

    GameCamera camera; 

    void calculate_fps(); //TODO: hacer

public:
    GameView();
    void update(const ActionDTO& action);
    void render();
};

#endif
