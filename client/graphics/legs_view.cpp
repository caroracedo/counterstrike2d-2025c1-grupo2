#include "legs_view.h"

LegsView::LegsView(SDL2pp::Texture& texture, uint32_t frame_duration_ms):
        walk_animation(
                texture,
                std::vector<SDL2pp::Rect>{SDL2pp::Rect(0, 0, 32, 32), SDL2pp::Rect(32, 0, 32, 32),
                                          SDL2pp::Rect(64, 0, 32, 32)},
                frame_duration_ms) {}

// std::vector<SDL2pp::Rect> LegsView::init_rects() {
//     std::vector<SDL2pp::Rect> rects;

//     for (int i = 0; i < 64; i += 32) {
//         rects.push_back(SDL2pp::Rect(i, LEGS_WIDTH, LEGS_WIDTH, LEGS_HEIGHT));
//     }

//     return rects;
// }

void LegsView::update(float x, float y) {
    pos_x = x;
    pos_y = y;
    walk_animation.update();
}

void LegsView::draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
    if (!camera.is_visible(pos_x, pos_y))
        return;
    float screenX = pos_x - camera.get_x();
    float screenY = pos_y - camera.get_y();

    // float centerX = screenX + 32 / 2.0f;
    // float centerY = screenY + 32 / 2.0f;

    // int mouseX, mouseY;
    // SDL_GetMouseState(&mouseX, &mouseY);
    // float dx = mouseX - centerX;
    // float dy = mouseY - centerY;
    // float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

    walk_animation.draw(renderer, screenX - LEGS_WIDTH / 2, screenY - LEGS_HEIGHT / 2, 40, 40,
                        angle, SDL_Point{16, 16});
}
