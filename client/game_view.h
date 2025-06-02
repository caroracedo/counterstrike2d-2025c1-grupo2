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
#include "player_HUD.h"


struct ObstacleView {
    float x, y, w, h;
    bool use_texture2 = false; 
};

class GameView {
private:
    SDL sdl;
    Window window;
    Renderer renderer;

    Texture terrorist_sprites;
    Texture counterterrorist_srpites;
    Texture legs_sprites;   
    
    Texture background;
    
    Texture box_texture;
    Texture box_texture2;
    
    Texture hud_numbres;
    
    Texture gun_texture;
        
    Uint32 last_frame_time;
    const Uint32 frame_delay = 1000 / 60;  // 60 FPS
    
    GameCamera camera; 
    PLayerHUD hud_view;
    std::unordered_map<uint16_t, std::unique_ptr<PlayerView>> players;
    std::unordered_map<uint16_t, std::unique_ptr<LegsView>> legs;
    std::unordered_map<uint16_t, std::unique_ptr<GunView>> guns;
    std::vector<ObstacleView> obstacles;  
    std::vector<BulletView> bullets; 
    
    //posible eliminación
    float last_px = -1;
    float last_py = -1;
    uint16_t local_id = 0;

    
    void update_obstacles(const ObjectDTO& object);
    void update_player(const ObjectDTO& object);
    void update_bullets(const ObjectDTO& object);

    void calculate_fps(); //TODO: hacer

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
