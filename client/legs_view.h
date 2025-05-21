#ifndef LEGS_VIEW_H
#define LEGS_VIEW_H

#include <SDL2pp/SDL2pp.hh>
#include <vector>
#include "animation.h"

using namespace SDL2pp;

class LegsView {
private:
    Animation walk_animation;
    float posX, posY;

public:
    LegsView(Texture& texture, std::vector<Rect> frames, Uint32 frame_duration_ms)
        : walk_animation(texture, std::move(frames), frame_duration_ms), posX(0), posY(0) {}
        
    void update_position(float x, float y) {
        posX = x;
        posY = y;
    }

    void update_animation() {
        walk_animation.update();
    }

    void draw(Renderer& renderer) {
        float centerX = posX + 32 / 2.0f;
        float centerY = posY + 32 / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float dx = mouseX - centerX;
        float dy = mouseY - centerY;
        float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

        walk_animation.draw(renderer, posX, posY, 40, 40, angle, SDL_Point{16, 16});
    }
};
#endif