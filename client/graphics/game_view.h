#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "common/action_DTO.h"
#include "common/constants.h"
#include "common/obstacle.h"

#include "FOV_view.h"
#include "bomb_view.h"
#include "bullet_view.h"
#include "drop_view.h"
#include "game_camera.h"
#include "gun_view.h"
#include "legs_view.h"
#include "obstacle_view.h"
#include "player_HUD.h"
#include "player_view.h"
#include "shop_view.h"
#include "sound_manager.h"
#include "stats_view.h"
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
    StatsView stats_view;
    FOVView fov_view;

    std::unordered_map<uint16_t, std::unique_ptr<PlayerView>> players;

    std::vector<DropView> drops;
    std::unordered_map<uint16_t, PlayerType> types;
    std::vector<ObstacleView> obstacles;
    std::vector<BulletView> bullets;
    std::vector<SDL_Rect> bomb_zones;

    uint16_t local_id = 0;
    bool show_pre_lobby = true;
    bool game_ended = false;
    WinnerTeamType winner = WinnerTeamType::UNKNOWN;
    TerrainType terrain;
    std::unordered_map<TerrainType, std::string> terrains;

    bool sounds_played = false;
    bool is_first_update = true;
    bool is_alive = true;


    void init_terrains();

    void update_obstacles(const ObjectDTO& object);
    void update_player(const ObjectDTO& object);
    void update_bullets(const ObjectDTO& object);
    void update_drops(const ObjectDTO& object);
    void update_bomb(const ObjectDTO& object);
    void update_bombzone(const ObjectDTO& object);
    void render_cursor();

    void handle_shop_action();
    void handle_stats_action(const ActionDTO& action);


public:
    GameView();
    void update(const ActionDTO& action);

    void render();

    void set_id(uint16_t id);

    GameCamera& get_camera();

    ShopView& get_shop();

    void pre_lobby(bool flag);

    void set_terrain(TerrainType type);

    void handle_attack();

    bool can_player_attack();

    void end_game(WinnerTeamType winner);

    void frame_sync();

    std::vector<float> player_position();
};

#endif
