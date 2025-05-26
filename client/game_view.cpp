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
                    Rect(0,  0, 32, 32),   
                    Rect(32, 0, 32, 32),   
                    Rect(64, 0, 32, 32)    
                },
                100),
    background (renderer, Surface(SDL_LoadBMP("../dustroof.bmp"))),
    camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2048, 2048),
    gun_texture(renderer, Surface(SDL_LoadBMP("../assets/gfx/weapons/ak47.bmp"))),
    gun_view(gun_texture) {}


void GameView::update(const ActionDTO2& action) {
    if (action.type != ActionType::UPDATE) return;
    
    for(const auto& object : action.objects) {
        
        float x = object.position[0];
        float y = object.position[1];
        if (object.type == ObjectType::PLAYER) {
            legs_view.update_position(x, y);
            legs_view.update_animation();
            player_view.update_position(x, y);
        }
        else if (object.type == ObjectType::OBSTACLE) {
            // obstacles.push_back({x, y});
            continue;
        }
    }    
}

void GameView::render() {
    float px = player_view.get_x();
    float py = player_view.get_y();
    camera.center_on(px + PLAYER_WIDTH / 2, 
                 py + PLAYER_HEIGHT / 2);


    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.Clear();

    Rect view = camera.get_viewport();

    renderer.Copy(background,view, Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));      

    SDL_Rect rojo = { 250 - camera.get_x(), 250 - camera.get_y(), 50, 50};
    renderer.SetDrawColor( 255, 0, 0, 255);
    renderer.FillRect(rojo);

    legs_view.draw(renderer,camera);
    
    for (auto& bullet : bullets)
        bullet.draw(renderer, camera);

    player_view.draw(renderer,camera);
    float angle_deg = player_view.get_angle();
    gun_view.draw(renderer, camera, px, py, angle_deg);
    
    renderer.Present();

    SDL_Delay(16);  // TODO: hacer calculo de FPS no hardcodeado.
}
