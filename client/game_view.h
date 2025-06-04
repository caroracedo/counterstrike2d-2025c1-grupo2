#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/action_DTO.h"
#include "../common/obstacle.h"

#include "bomb_view.h"
#include "bullet_view.h"
#include "game_camera.h"
#include "gun_view.h"
#include "legs_view.h"
#include "player_HUD.h"
#include "player_view.h"


struct ObstacleView {
    float x, y, w, h;
    bool use_texture2 = false;
};

class GameView {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    SDL2pp::Texture terrorist_sprites;
    SDL2pp::Texture counterterrorist_srpites;
    SDL2pp::Texture legs_sprites;

    SDL2pp::Texture background;

    SDL2pp::Texture box_texture;
    SDL2pp::Texture box_texture2;

    SDL2pp::Texture hud_numbres;

    SDL2pp::Texture gun_texture;

    SDL2pp::Texture bomb_texture;
    SDL2pp::Texture explotion_sprites;

    uint32_t last_frame_time = 0;
    const uint32_t frame_delay = 1000 / 60;  // 60 FPS

    // SoundManager sound_manager;
    GameCamera camera;
    PLayerHUD hud_view;
    BombView bomb_view;

    std::unordered_map<uint16_t, std::unique_ptr<PlayerView>> players;
    std::unordered_map<uint16_t, std::unique_ptr<LegsView>> legs;
    std::unordered_map<uint16_t, std::unique_ptr<GunView>> guns;
    std::vector<ObstacleView> obstacles;
    std::vector<BulletView> bullets;

    // posible eliminación
    float last_px = -1;
    float last_py = -1;
    uint16_t local_id = 0;


    void update_obstacles(const ObjectDTO& object);
    void update_player(const ObjectDTO& object);
    void update_bullets(const ObjectDTO& object);

    void calculate_fps();  // TODO: hacer

public:
    GameView();
    void update(const ActionDTO& action);
    void render();

    void set_id(uint16_t id) {
        local_id = id;
        std::cout << "local_id: " << local_id << std::endl;
    }

    // void update_graphics(const ActionDTO& action);

    void frame_sync();
};

#endif
