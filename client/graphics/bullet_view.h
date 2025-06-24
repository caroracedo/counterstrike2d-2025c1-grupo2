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
    float x;
    float y;

public:
    BulletView(float start_x, float start_y);
    void update(float new_x, float new_y);
    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera);
};

#endif
