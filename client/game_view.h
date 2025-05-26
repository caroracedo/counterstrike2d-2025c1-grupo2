#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "player_view.h"
#include "../common/action_DTO.h"
#include "legs_view.h"
#include "game_camera.h"
#include "bullet_view.h"
#include "gun_view.h"

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
    Texture gun_texture;
    GunView gun_view;

    std::vector<BulletView> bullets; 
    void calculate_fps(); //TODO: hacer

public:
    GameView();
    void update(const ActionDTO& action);
    void render();
    void shoot(float angle_deg) {
        float px = player_view.get_x() + PLAYER_WIDTH / 2;
        float py = player_view.get_y() + PLAYER_HEIGHT / 2;
        bullets.emplace_back(px, py, angle_deg);
    }

    void update_bullets() {
        for (auto& bullet : bullets)
            bullet.update();

        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [](const BulletView& b) { return !b.is_active(); }),
            bullets.end()
        );
    }
    
    void update_graphics(const ActionDTO& action);
};

#endif
