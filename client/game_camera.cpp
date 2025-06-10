#include "game_camera.h"

GameCamera::GameCamera(float screen_w, float screen_h, float map_w, float map_h):
        x(0),
        y(0),
        screen_width(screen_w),
        screen_height(screen_h),
        map_width(map_w),
        map_height(map_h) {}


void GameCamera::center_on(float target_x, float target_y) {
    x = target_x - screen_width / 2.0f;
    y = target_y - screen_height / 2.0f;
}

SDL2pp::Rect GameCamera::get_viewport() const {
    return SDL2pp::Rect(static_cast<int>(x), static_cast<int>(y), screen_width, screen_height);
}
