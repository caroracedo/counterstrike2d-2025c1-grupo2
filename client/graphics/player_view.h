#ifndef PLAYER_VIEW_H
#define PLAYER_VIEW_H

#include <cmath>
#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/types.h"

#include "game_camera.h"
#include "sound_manager.h"
#include "textures_manager.h"

class PlayerView {
private:
    TextureManager& texture_manager;

    SoundManager& sound_manager;

    uint16_t life = 0;

    uint16_t id;

    float posX = 0.0f, posY = 0.0f;

    float angle = 0.0f;

    SDL2pp::Rect current_frame;

    std::unordered_map<PlayerSkin, std::string> player_skins;

    PlayerType type = PlayerType::TERRORIST;

    PlayerSkin skin = PlayerSkin::SEALFORCE;


public:
    explicit PlayerView(TextureManager& texture_manager, SoundManager& sound_manager, uint16_t id,
                        PlayerSkin skin);

    void initialize_resources();

    bool update_position(float x, float y, uint16_t life);

    void update_styles(PlayerType new_type, WeaponModel new_posture);

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera);

    float get_angle() const;

    float get_x() const;

    float get_y() const;

    void update_angle(float angle) { this->angle = 360.0f - (angle - 90.0f); }
};

#endif
