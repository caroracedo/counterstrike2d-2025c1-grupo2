#ifndef LEGS_VIEW_H
#define LEGS_VIEW_H

#include <utility>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "animation.h"

class LegsView {
private:
    Animation walk_animation;
    float posX, posY;

public:
    LegsView(SDL2pp::Texture& texture, std::vector<SDL2pp::Rect> frames,
             uint32_t frame_duration_ms):
            walk_animation(texture, std::move(frames), frame_duration_ms), posX(0), posY(0) {}

    void update_position(float x, float y) {
        posX = x;
        posY = y;
    }

    void update_animation() { walk_animation.update(); }

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
        float screenX = posX - camera.get_x();
        float screenY = posY - camera.get_y();

        float centerX = screenX + 32 / 2.0f;
        float centerY = screenY + 32 / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float dx = mouseX - centerX;
        float dy = mouseY - centerY;
        float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

        walk_animation.draw(renderer, screenX, screenY, 40, 40, angle, SDL_Point{16, 16});
    }
};
#endif
