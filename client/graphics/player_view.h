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
#include "textures_manager.h"

class PlayerView {
private:
    float posX = 0.0f, posY = 0.0f;

    PlayerType type = PlayerType::TERRORIST;

    SDL2pp::Rect current_frame;

    float angle = 0.0f;

    TextureManager& texture_manager;

    std::unordered_map<PlayerType, std::string> player_types;

    uint16_t id;

public:
    explicit PlayerView(TextureManager& texture_manager, uint16_t id);

    bool update_position(float x, float y, float angle);

    void update_styles(PlayerType new_type, WeaponModel new_posture);

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera);

    float get_angle() const;

    float get_x() const;

    float get_y() const;
};

#endif
