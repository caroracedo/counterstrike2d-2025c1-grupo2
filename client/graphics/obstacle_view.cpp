#include "obstacle_view.h"

ObstacleView::ObstacleView(SDL2pp::Renderer& renderer, TextureManager& textures):
        renderer(renderer), texture_manager(textures) {
    boxes[ObstacleType::OBSTACLE1] = "box_1";
    boxes[ObstacleType::OBSTACLE2] = "box_2";
    boxes[ObstacleType::OBSTACLE3] = "box_3";
    boxes[ObstacleType::OBSTACLE4] = "box_4";
    boxes[ObstacleType::OBSTACLE5] = "box_5";
}

void ObstacleView::update(float x, float y, float w, float h, ObstacleType type) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->type = type;
}

void ObstacleView::draw(const GameCamera& camera) {
    if (!camera.is_visible(x, y, w, h))
        return;
    float screenX = x - camera.get_x();
    float screenY = y - camera.get_y();

    SDL2pp::Rect dst_rect = {static_cast<int>(screenX), static_cast<int>(screenY), int(w), int(h)};
    SDL2pp::Texture& texture = *texture_manager.get_texture(boxes[type]);
    SDL_Rect src_rect = {0, 0, int(w), int(h)};

    renderer.Copy(texture, src_rect, dst_rect);
}
