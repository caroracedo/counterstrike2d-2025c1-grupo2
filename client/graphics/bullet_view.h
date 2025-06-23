#ifndef BULLET_VIEW_H
#define BULLET_VIEW_H

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "common/action_DTO.h"

#include "game_camera.h"

#define BULLET_WIDTH 4
#define BULLET_HEIGHT 4

class BulletView {
private:
    // int id;
    float x;
    float y;
    // float angle;

public:
    BulletView(float start_x, float start_y): x(start_x), y(start_y) {}

    void update(float new_x, float new_y) {
        x = new_x;
        y = new_y;
    }

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
        if (!camera.is_visible(x, y, BULLET_WIDTH, BULLET_HEIGHT))
            return;
        SDL2pp::Rect rect = {static_cast<int>(x - camera.get_x()),
                             static_cast<int>(y - camera.get_y()), BULLET_WIDTH, BULLET_HEIGHT};
        renderer.SetDrawColor(0, 0, 0, 255);
        renderer.FillRect(rect);
    }
};

#endif
