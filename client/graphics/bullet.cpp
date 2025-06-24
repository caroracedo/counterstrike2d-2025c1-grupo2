#include "bullet_view.h"

BulletView::BulletView(float start_x, float start_y): x(start_x), y(start_y) {}

void BulletView::update(float new_x, float new_y) {
    x = new_x;
    y = new_y;
}

void BulletView::draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
    if (!camera.is_visible(x, y, BULLET_WIDTH, BULLET_HEIGHT))
        return;
    SDL2pp::Rect rect = {static_cast<int>(x - camera.get_x()), static_cast<int>(y - camera.get_y()),
                         BULLET_WIDTH, BULLET_HEIGHT};
    renderer.SetDrawColor(0, 0, 0, 255);
    renderer.FillRect(rect);
}
