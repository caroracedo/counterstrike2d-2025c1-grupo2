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
#include "obstacle_view.h"
#include "player_HUD.h"
#include "player_view.h"
#include "shop_view.h"
#include "sound_manager.h"
#include "textures_manager.h"


class GameView {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    TextureManager texture_manager;

    uint32_t last_frame_time = 0;
    const uint32_t frame_delay = 1000 / 60;

    SoundManager sound_manager;
    GameCamera camera;
    PlayerHUD hud_view;
    BombView bomb_view;
    ShopView shop_view;

    std::unordered_map<uint16_t, std::unique_ptr<PlayerView>> players;
    std::unordered_map<uint16_t, std::unique_ptr<LegsView>> legs;
    std::unordered_map<uint16_t, std::unique_ptr<GunView>> guns;
    std::vector<ObstacleView> obstacles;
    std::vector<BulletView> bullets;
    std::vector<SDL_Rect> bomb_zones;

    uint16_t local_id = 0;


    void update_obstacles(const ObjectDTO& object);
    void update_player(const ObjectDTO& object);
    void update_bullets(const ObjectDTO& object);

public:
    GameView();
    void update(const ActionDTO& action);

    void render();

    void set_id(uint16_t id) { local_id = id; }

    GameCamera& get_camera() { return camera; }

    ShopView& get_shop() { return shop_view; }

    void frame_sync();
};

#endif
