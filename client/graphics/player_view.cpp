#include "player_view.h"

PlayerView::PlayerView(TextureManager& texture_manager, SoundManager& sound_manager, uint16_t id,
                       PlayerSkin skin, SDL2pp::Renderer& renderer):
        texture_manager(texture_manager),
        sound_manager(sound_manager),
        legs_view(*texture_manager.get_texture("legs"), 100),
        gun_view(renderer),
        id(id),
        skin(skin) {
    // std::cout << "Player created with ID: " << id << std::endl;
    initialize_resources();
}

void PlayerView::initialize_resources() {
    player_skins[PlayerSkin::SEALFORCE] = "ct1";
    player_skins[PlayerSkin::GERMANGSG9] = "ct2";
    player_skins[PlayerSkin::UKSAS] = "ct3";
    player_skins[PlayerSkin::FRENCHGIGN] = "ct4";
    player_skins[PlayerSkin::PHEONIX] = "t1";
    player_skins[PlayerSkin::L337KREW] = "t2";
    player_skins[PlayerSkin::ARTICAVENGER] = "t3";
    player_skins[PlayerSkin::GUERRILLA] = "t4";
}

bool PlayerView::update_position(float x, float y, uint16_t new_life) {
    const float epsilon = 0.01f;

    if (new_life < life) {
        sound_manager.play("hit");
    }

    this->life = new_life;


    if (std::abs(posX - x) < epsilon && std::abs(posY - y) < epsilon) {
        return false;
    }

    posX = x;
    posY = y;
    return true;
}

void PlayerView::update_styles(PlayerType new_type, WeaponModel new_posture) {
    type = new_type;
    if (new_posture == WeaponModel::GLOCK) {
        current_frame = SDL2pp::Rect{32, 32, 32, 32};
    } else if (new_posture == WeaponModel::KNIFE) {
        current_frame = SDL2pp::Rect{0, 0, 32, 32};
    } else {
        current_frame = SDL2pp::Rect{0, 64, 32, 32};
    }
}

void PlayerView::draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
    if (!camera.is_visible(posX, posY))
        return;

    float screenX = posX - camera.get_x();
    float screenY = posY - camera.get_y();
    float draw_angle = angle;

    for (const auto& effect: active_effects) {
        auto [dx, dy] = effect->get_offset(angle);
        screenX += dx;
        screenY += dy;
        legs_view.update(posX + dx, posY + dy);
        gun_view.update(posX + dx, posY + dy, gun_view.get_current_type());
        draw_angle += effect->get_rotation_offset();
    }

    SDL2pp::Texture& texture = *texture_manager.get_texture(player_skins[skin]);

    SDL2pp::Rect dstRect{static_cast<int>(screenX) - PLAYER_WIDTH / 2,
                         static_cast<int>(screenY) - PLAYER_HEIGHT / 2, PLAYER_WIDTH,
                         PLAYER_HEIGHT};

    SDL_Point center{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2};


    legs_view.draw(renderer, camera, draw_angle);
    renderer.Copy(texture, current_frame, dstRect, draw_angle, center, SDL_FLIP_NONE);
    gun_view.draw(renderer, camera, draw_angle);

    for (auto& effect: active_effects) {
        effect->update();
    }

    active_effects.erase(std::remove_if(active_effects.begin(), active_effects.end(),
                                        [](const auto& eff) { return !eff->is_active(); }),
                         active_effects.end());
}


float PlayerView::get_x() const { return posX; }

float PlayerView::get_y() const { return posY; }

float PlayerView::get_angle() const { return angle; }
