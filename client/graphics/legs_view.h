#ifndef LEGS_VIEW_H
#define LEGS_VIEW_H

#include <utility>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "animation.h"
#include "game_camera.h"

#define LEGS_WIDTH 32
#define LEGS_HEIGHT 32

class LegsView {
private:
    Animation walk_animation;
    float pos_x = 0, pos_y = 0;

    // std::vector<SDL2pp::Rect> init_rects();

public:
    LegsView(SDL2pp::Texture& texture, uint32_t frame_duration_ms);

    void update(float x, float y);

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera);
};
#endif
