#include "gun_view.h"

#include <cmath>

GunView::GunView(TextureManager& texture_manager, SoundManager& sound_manager):
        texture_manager(texture_manager), sound_manager(sound_manager) {
    weapons[WeaponModel::GLOCK] = "glock";
    weapons[WeaponModel::AK47] = "ak47";
    weapons[WeaponModel::M3] = "m3";
    weapons[WeaponModel::AWP] = "awp";
    weapons[WeaponModel::KNIFE] = "knife";
    weapons[WeaponModel::BOMB] = "bomb";
}

void GunView::update(float px, float py, WeaponModel type) {
    x = px;
    y = py;
    current_type = type;
}

void GunView::draw(SDL2pp::Renderer& renderer, const GameCamera& camera, float angle) {
    if (!camera.is_visible(x, y, GUN_WIDTH, GUN_HEIGHT))
        return;

    if (current_type == WeaponModel::UNKNOWN) {
        return;
    }

    auto pos = get_gun_tip_screen_position(camera, angle);

    float gunX = pos.first;
    float gunY = pos.second;

    renderer.Copy(*texture_manager.get_texture(weapons[current_type]),
                  SDL2pp::Rect(0, 0, GUN_WIDTH, GUN_HEIGHT),
                  SDL2pp::Rect(static_cast<int>(gunX - GUN_WIDTH),
                               static_cast<int>(gunY - GUN_HEIGHT), GUN_WIDTH, GUN_HEIGHT),
                  angle, SDL_Point{GUN_WIDTH / 2, GUN_HEIGHT / 2}, SDL_FLIP_NONE);
}

std::pair<float, float> GunView::get_gun_tip_screen_position(const GameCamera& camera,
                                                             float angle) const {
    float screenX = x - camera.get_x();
    float screenY = y - camera.get_y();

    float centerX = screenX + PLAYER_WIDTH / 2.0f;
    float centerY = screenY + PLAYER_HEIGHT / 2.0f;

    const float baseOffsetX = 0.0f;
    const float baseOffsetY = -15.0f;

    float rad = angle * M_PI / 180.0f;

    float rotatedOffsetX = baseOffsetX * cos(rad) - baseOffsetY * sin(rad);
    float rotatedOffsetY = baseOffsetX * sin(rad) + baseOffsetY * cos(rad);

    float gunX = centerX + rotatedOffsetX;
    float gunY = centerY + rotatedOffsetY;

    return {gunX, gunY};
}

WeaponModel GunView::get_current_type() const { return current_type; }

uint32_t GunView::get_weapon_cooldown(WeaponModel model) {
    switch (model) {
        case WeaponModel::AWP:
            return 1500;
        case WeaponModel::GLOCK:
            return 100;
        case WeaponModel::AK47:
            return 200;
        case WeaponModel::M3:
            return 800;
        case WeaponModel::KNIFE:
            return 0;
        default:
            return 150;  // valor por defecto
    }
}
