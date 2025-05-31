#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <unordered_map>
#include <cstdint>
#include <memory>

#include "player_view.h"
#include "../common/action_DTO.h"
#include "../common/obstacle.h"
#include "legs_view.h"
#include "game_camera.h"
#include "bullet_view.h"
#include "gun_view.h"


struct ObstacleView {
    float x, y, w, h;
    bool use_texture2 = false; 
};

class GameView {
private:
    SDL sdl;
    Window window;
    Renderer renderer;

    Texture body_sprites;
    Texture legs_sprites;   

    std::unordered_map<uint8_t, std::unique_ptr<PlayerView>> players;
    std::unordered_map<uint8_t, std::unique_ptr<LegsView>> legs;

    int local_id;

    PlayerView player_view; 
    LegsView legs_view;

    Texture background;

    GameCamera camera; 
    // Texture gun_texture;
    // GunView gun_view;

    Texture box_texture;
    Texture box_texture2;
    
    Uint32 last_frame_time;
    const Uint32 frame_delay = 1000 / 60;  // 60 FPS

    std::vector<ObstacleView> obstacles;  
    std::vector<BulletView> bullets; 
    float last_px = -1;
    float last_py = -1;
    
    void update_obstacles(const ObjectDTO& object);
    void update_player(const ObjectDTO& object);
    void update_bullets(const ObjectDTO& object);

    void calculate_fps(); //TODO: hacer

public:
    GameView();
    void update(const ActionDTO& action);
    void render();
    
    void update_graphics(const ActionDTO& action);

    void frame_sync();
};

#endif
