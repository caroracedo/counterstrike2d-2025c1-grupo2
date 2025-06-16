#ifndef GUN_VIEW_H
#define GUN_VIEW_H

#include <cmath>
#include <memory>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/types.h"

#include "game_camera.h"

// struct GunSpriteData {
//     SDL2pp::Texture* texture;
//     SDL_Rect sprite_clip;
//     int width, height;
//     float offset_x, offset_y;
// };

class GunView {
private:
    std::unordered_map<WeaponModel, std::unique_ptr<SDL2pp::Texture>> gun_sprites;
    WeaponModel current_type = WeaponModel::GLOCK;

    float x = 0, y = 0;
    float angle = 0.0f;  // Grados

public:
    explicit GunView(SDL2pp::Renderer& renderer) {
        gun_sprites[WeaponModel::GLOCK] = std::make_unique<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP(ASSETS_PATH "/gfx/weapons/glock.bmp")));
        gun_sprites[WeaponModel::AK47] = std::make_unique<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP(ASSETS_PATH "/gfx/weapons/ak47.bmp")));
        gun_sprites[WeaponModel::M3] = std::make_unique<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP(ASSETS_PATH "/gfx/weapons/m3.bmp")));
        gun_sprites[WeaponModel::AWP] = std::make_unique<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP(ASSETS_PATH "/gfx/weapons/awp.bmp")));
        gun_sprites[WeaponModel::KNIFE] = std::make_unique<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP(ASSETS_PATH "/gfx/weapons/knife.bmp")));
    }

    void change_gun(WeaponModel type) { current_type = type; }

    void update(float px, float py) {
        x = px;
        y = py;
    }


    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
        if (!camera.is_visible(x, y))
            return;
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        float centerX = screenX + PLAYER_WIDTH / 2.0f;
        float centerY = screenY + PLAYER_HEIGHT / 2.0f;

        // int mouseX, mouseY;
        // SDL_GetMouseState(&mouseX, &mouseY);

        // float dx = mouseX - centerX;
        // float dy = mouseY - centerY;

        // float new_angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;
        // this->angle = new_angle;

        // Offset de la mano en reposo (cuando apunta hacia arriba)
        const float baseOffsetX = 0.0f;
        const float baseOffsetY = -15.0f;

        float rad = angle * M_PI / 180.0f;

        float rotatedOffsetX = baseOffsetX * cos(rad) - baseOffsetY * sin(rad);
        float rotatedOffsetY = baseOffsetX * sin(rad) + baseOffsetY * cos(rad);

        float gunX = centerX + rotatedOffsetX;
        float gunY = centerY + rotatedOffsetY;

        renderer.Copy(*gun_sprites[current_type], SDL2pp::Rect(0, 0, GUN_WIDTH, GUN_HEIGHT),
                      SDL2pp::Rect(static_cast<int>(gunX - GUN_WIDTH),
                                   static_cast<int>(gunY - GUN_HEIGHT), GUN_WIDTH, GUN_HEIGHT),
                      angle, SDL_Point{GUN_WIDTH / 2, GUN_HEIGHT / 2}, SDL_FLIP_NONE);
    }

    void update_angle(float angle) { this->angle = angle; }
};

#endif
