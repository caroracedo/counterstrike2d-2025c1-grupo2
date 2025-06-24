#ifndef PLAYER_VIEW_H
#define PLAYER_VIEW_H

#include <cmath>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/object_DTO.h"
#include "common/types.h"
#include "vfx/kickback.h"
#include "vfx/knife_swing.h"
#include "vfx/visual_effect.h"

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

    float posX = 0.0f, posY = 0.0f;

    float angle = 0.0f;

    uint32_t last_attack_time = 0;

    SDL2pp::Rect current_frame;

    std::vector<std::unique_ptr<VisualEffect>> active_effects;

    std::unordered_map<PlayerSkin, std::string> player_skins;

    PlayerType type = PlayerType::TERRORIST;

    PlayerSkin skin = PlayerSkin::SEALFORCE;

    void initialize_resources();

    bool update_position(float x, float y, uint16_t life);

    void update_styles(PlayerType new_type, WeaponModel new_posture, PlayerSkin new_skin);

    void update_angle(float angle);

public:
    explicit PlayerView(TextureManager& texture_manager, SoundManager& sound_manager, uint16_t id,
                        PlayerSkin skin);


    float get_angle() const;

    float get_x() const;

    float get_y() const;

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera);

    void start_knife_animation();

    void start_kickback();

    bool update(const ObjectDTO& object);

    WeaponModel get_current_weapon() const;

    bool can_attack();
};

#endif
