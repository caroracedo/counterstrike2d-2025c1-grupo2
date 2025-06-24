#include "game_camera.h"

GameCamera::GameCamera(float screen_w, float screen_h, float map_w, float map_h):
        x(0),
        y(0),
        screen_width(static_cast<int>(screen_w)),
        screen_height(static_cast<int>(screen_h)),
        map_width(map_w),
        map_height(map_h) {}

void GameCamera::center_on(float target_x, float target_y) {
    x = target_x - screen_width / 2.0f;
    y = target_y - screen_height / 2.0f;
}

std::vector<float> GameCamera::world_to_screen(float worldX, float worldY) {
    return std::vector<float>{worldX - x, worldY - y};
}

SDL2pp::Rect GameCamera::get_viewport() const {
    return SDL2pp::Rect(static_cast<int>(x), static_cast<int>(y), screen_width, screen_height);
}

float GameCamera::get_x() const { return x; }

float GameCamera::get_y() const { return y; }

bool GameCamera::is_visible(float worldX, float worldY, float obj_width, float obj_height) const {
    return worldX + obj_width > x && worldX < x + screen_width && worldY + obj_height > y &&
           worldY < y + screen_height;
}
