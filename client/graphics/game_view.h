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


    void update_obstacles(const ObjectDTO& object);
    void update_player(const ObjectDTO& object);
    void update_bullets(const ObjectDTO& object);
    void update_drops(const ObjectDTO& object);
    void render_cursor();

public:
    GameView();
    void update(const ActionDTO& action);

    void render();

    void set_id(uint16_t id) { local_id = id; }

    GameCamera& get_camera() { return camera; }

    ShopView& get_shop() { return shop_view; }

    void frame_sync();

    void pre_lobby(bool flag) { show_pre_lobby = flag; }

    void set_terrain(TerrainType type) { terrain = type; }

    void init_terrains() {
        terrains[TerrainType::PUEBLOAZTECA] = "aztec";
        terrains[TerrainType::ZONAENTRENAMIENTO] = "office";
        terrains[TerrainType::DESIERTO] = "sand1";
    }

    std::vector<uint16_t> player_position() {
        auto it = players.find(local_id);
        if (it != players.end() && it->second) {
            uint16_t x = static_cast<uint16_t>(it->second->get_x());
            uint16_t y = static_cast<uint16_t>(it->second->get_y());
            return {x, y};
        }
        return {};
    }

    void handle_attack() {
        if (is_alive) {
            WeaponModel current_weapon = players[local_id]->get_current_weapon();
            // std::cout << "Current weapon: " << static_cast<int>(current_weapon) << std::endl;

            if (current_weapon == WeaponModel::KNIFE) {
                players[local_id]->start_knife_animation();
                sound_manager.playWithCooldown("knife_slash", 150, 0);
            } else {
                players[local_id]->start_kickback();
                sound_manager.playWithCooldown("bullet", 150, 0);
            }
        }
    }

    bool can_player_attack() { return players[local_id]->can_attack(); }

    void end_game(WinnerTeamType winner) {
        stats_view.set_visible(false);
        this->winner = winner;
        game_ended = true;
    }
};

#endif
