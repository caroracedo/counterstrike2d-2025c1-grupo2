#include "game_camera.h"

GameCamera::GameCamera(int screen_w, int screen_h, int map_w, int map_h):
        x(0),
        y(0),
        screen_width(screen_w),
        screen_height(screen_h),
        map_width(map_w),
        map_height(map_h) {}


void GameCamera::center_on(int target_x, int target_y) {
    x = target_x - screen_width / 2;
    y = target_y - screen_height / 2;

    // if (x < 0) x = 0;

    // if (y < 0) y = 0;

    if (x > map_width - screen_width)
        x = map_width - screen_width;

    if (y > map_height - screen_height)
        y = map_height - screen_height;
}

SDL2pp::Rect GameCamera::get_viewport() const {
    return SDL2pp::Rect(x, y, screen_width, screen_height);
}
