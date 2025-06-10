#include "bomb_view.h"

BombView::BombView(SDL2pp::Texture& bomb_texture, SDL2pp::Texture& explotion, SoundManager& sm):
        explotion_animation(explotion, get_rects(), 30),
        bomb_texture(bomb_texture),
        x(0),
        y(0),
        sounds(sm) {}

std::vector<SDL2pp::Rect> BombView::get_rects() {
    std::vector<SDL2pp::Rect> rects;

    for (int j = 0; j < 320; j += 64) {
        for (int i = 0; i < 320; i += 64) {
            rects.push_back(SDL2pp::Rect(i, j, 64, 64));
        }
    }

    return rects;
}

void BombView::update(float px, float py) {
    x = px;
    y = py;
}

void BombView::draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
    float screenX = x - camera.get_x();
    float screenY = y - camera.get_y();
    SDL2pp::Rect dst_rect = {static_cast<int>(screenX), static_cast<int>(screenY), BOMB_WIDTH,
                             BOMB_HEIGHT};

    SDL2pp::Rect src_rect = {0, 0, BOMB_WIDTH, BOMB_HEIGHT};

    renderer.Copy(bomb_texture, src_rect, dst_rect);
}

void BombView::drawExplosion(SDL2pp::Renderer& renderer, const GameCamera& camera) {
    float screenX = x - camera.get_x();
    float screenY = y - camera.get_y();

    for (int i = 0; i < 10; ++i) {
        explotion_animation.update();
        explotion_animation.draw(renderer, screenX, screenY, BOMB_WIDTH, BOMB_HEIGHT, 0.0,
                                 SDL_Point{16, 16});
    }
}

void BombView::activate_bomb() { active = true; }

void BombView::explode() { exploding = true; }

void BombView::reset() {
    exploding = false;
    active = false;
}

bool BombView::is_exploding() const { return exploding; }

bool BombView::is_active() const { return active; }
