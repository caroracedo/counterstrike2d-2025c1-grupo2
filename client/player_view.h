#ifndef PLAYER_VIEW_H
#define PLAYER_VIEW_H

#include <cmath>
#include <memory>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "../common/constants.h"
#include "../common/types.h"

#include "game_camera.h"

class PlayerView {
private:
    float posX = 0.0f, posY = 0.0f;
    // SDL2pp::Texture& cuerpo;
    PlayerType type = PlayerType::TERRORIST;
    SDL2pp::Rect current_frame;
    std::unordered_map<PlayerType, std::unique_ptr<SDL2pp::Texture>> player_sprites;
    float angle = 0.0f;

public:
    // explicit PlayerView(SDL2pp::Texture& cuerpo): cuerpo(cuerpo) {}

    explicit PlayerView(SDL2pp::Renderer& renderer) {
        player_sprites[PlayerType::TERRORIST] = std::make_unique<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/player/t1.bmp")));
        player_sprites[PlayerType::COUNTERTERRORIST] = std::make_unique<SDL2pp::Texture>(
                renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp")));
    }

    bool update_position(float x, float y) {
        const float epsilon = 0.01f;
        if (std::abs(posX - x) < epsilon && std::abs(posY - y) < epsilon) {
            return false;
        }
        posX = x;
        posY = y;
        return true;
    }

    void update_styles(PlayerType new_type, WeaponModel new_posture) {
        type = new_type;
        if (new_posture == WeaponModel::GLOCK) {
            current_frame = SDL2pp::Rect{32, 32, 32, 32};
        } else if (new_posture == WeaponModel::KNIFE) {
            current_frame = SDL2pp::Rect{0, 0, 32, 32};
        } else {
            current_frame = SDL2pp::Rect{0, 64, 32, 32};
        }
    }

    float get_x() const { return posX; }
    float get_y() const { return posY; }

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
        float screenX = posX - camera.get_x();
        float screenY = posY - camera.get_y();

        float centerX = screenX + PLAYER_WIDTH / 2.0f;
        float centerY = screenY + PLAYER_HEIGHT / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        float dx = mouseX - centerX;
        float dy = mouseY - centerY;

        float new_angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;
        this->angle = new_angle;
        renderer.Copy(*player_sprites[type], current_frame,
                      SDL2pp::Rect(static_cast<int>(screenX), static_cast<int>(screenY),
                                   PLAYER_WIDTH, PLAYER_HEIGHT),
                      angle, SDL_Point{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2}, SDL_FLIP_NONE);
    }
    float get_angle() const { return angle; }
};

#endif
