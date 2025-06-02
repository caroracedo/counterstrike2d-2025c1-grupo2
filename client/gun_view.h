#ifndef GUN_VIEW_H
#define GUN_VIEW_H

#include <cmath>

#include <SDL2pp/SDL2pp.hh>

#include "../common/constants.h"

#include "game_camera.h"

#define GUN_WIDTH 32
#define GUN_HEIGHT 32

using namespace SDL2pp;

class GunView {
private:
    Texture& texture;
    float x, y;
    float angle = 0.0f;  // Grados

public:
    GunView(Texture& tex): texture(tex) {}

    void update(float px, float py) {
        x = px;
        y = py;
    }

    void update_angle(float new_angle) { angle = new_angle; }

    void draw(Renderer& renderer, GameCamera& camera) {
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        float centerX = screenX + PLAYER_WIDTH / 2.0f;
        float centerY = screenY + PLAYER_HEIGHT / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        float dx = mouseX - centerX;
        float dy = mouseY - centerY;

        float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;
        this->angle = angle;

        // Offset de la mano en reposo (cuando apunta hacia arriba)
        const float baseOffsetX = 0.0f;
        const float baseOffsetY = -15.0f;

        float rad = angle * M_PI / 180.0f;

        float rotatedOffsetX = baseOffsetX * cos(rad) - baseOffsetY * sin(rad);
        float rotatedOffsetY = baseOffsetX * sin(rad) + baseOffsetY * cos(rad);

        float gunX = centerX + rotatedOffsetX;
        float gunY = centerY + rotatedOffsetY;

        renderer.Copy(texture, Rect(0, 0, GUN_WIDTH, GUN_HEIGHT),
                      Rect(static_cast<int>(gunX - GUN_WIDTH / 2.0f),
                           static_cast<int>(gunY - GUN_HEIGHT / 2.0f), GUN_WIDTH, GUN_HEIGHT),
                      angle, SDL_Point{GUN_WIDTH / 2, GUN_HEIGHT / 2}, SDL_FLIP_NONE);
    }
};

#endif
