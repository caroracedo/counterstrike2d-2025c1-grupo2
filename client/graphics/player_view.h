#ifndef PLAYER_VIEW_H
#define PLAYER_VIEW_H

#include <cmath>
#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/object_DTO.h"
#include "common/types.h"

#include "game_camera.h"
#include "gun_view.h"
#include "legs_view.h"
#include "sound_manager.h"
#include "textures_manager.h"

class PlayerView {
private:
    TextureManager& texture_manager;

    SoundManager& sound_manager;

    LegsView legs_view;

    GunView gun_view;

    uint16_t life = 0;

    uint16_t id;

    uint32_t knife_start = 0;

    bool is_knife = false;

    float posX = 0.0f, posY = 0.0f;

    float angle = 0.0f;

    bool is_kicking_back = false;
    uint32_t kick_start_time = 0;
    const uint32_t KICKBACK_DURATION_MS = 100;

    SDL2pp::Rect current_frame;

    std::unordered_map<PlayerSkin, std::string> player_skins;

    PlayerType type = PlayerType::TERRORIST;

    PlayerSkin skin = PlayerSkin::SEALFORCE;


public:
    explicit PlayerView(TextureManager& texture_manager, SoundManager& sound_manager, uint16_t id,
                        PlayerSkin skin, SDL2pp::Renderer& renderer);

    void initialize_resources();

    bool update_position(float x, float y, uint16_t life);

    void update_styles(PlayerType new_type, WeaponModel new_posture);

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera);

    float get_angle() const;

    float get_x() const;

    float get_y() const;

    void update_angle(float angle) { this->angle = 360.0f - (angle - 90.0f); }

    void start_knife_animation() {
        is_knife = true;
        knife_start = SDL_GetTicks();
    }

    void start_kickback() {
        is_kicking_back = true;
        kick_start_time = SDL_GetTicks();
    }

    bool update(const ObjectDTO& object) {
        float new_angle = object.angle;
        float x = static_cast<float>(object.position[0]);
        float y = static_cast<float>(object.position[1]);

        bool moved = update_position(x, y, object.health);

        update_angle(new_angle);
        if (moved) {
            legs_view.update(x, y);
        }

        update_styles(object.player_type, object.weapon_model);
        gun_view.update(x, y, object.weapon_model);

        return moved;
    }

    bool has_knife_equipped() const { return gun_view.has_knife_equipped(); }
};

#endif
