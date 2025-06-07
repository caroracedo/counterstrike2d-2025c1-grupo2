#ifndef BOMB_VIEW_H
#define BOMB_VIEW_H

#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "../common/constants.h"

#include "animation.h"
#include "game_camera.h"

#define BOMB_WIDTH 32
#define BOMB_HEIGHT 32

class BombView {
private:
    Animation explotion_animation;
    SDL2pp::Texture& texture;
    float x, y;
    bool exploding = false;
    bool active = false;

    std::vector<SDL2pp::Rect> get_rects() {
        std::vector<SDL2pp::Rect> rects;

        for (int j = 0; j < 320; j += 64) {
            for (int i = 0; i < 320; i += 64) {
                rects.push_back(SDL2pp::Rect(i, j, 64, 64));
            }
        }

        return rects;
    }

public:
    BombView(SDL2pp::Texture& tex, SDL2pp::Texture& explotion):
            explotion_animation(explotion, get_rects(), 25), texture(tex), x(0), y(0) {}

    void update(float px, float py) {
        x = px;
        y = py;
    }

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
        // Dibuja la bomba en la posición ajustada por la cámara
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();
        SDL2pp::Rect dst_rect = {static_cast<int>(screenX), static_cast<int>(screenY), BOMB_WIDTH,
                                 BOMB_HEIGHT};

        SDL2pp::Rect src_rect = {0, 0, BOMB_WIDTH, BOMB_HEIGHT};

        renderer.Copy(texture, src_rect, dst_rect);
    }

    void drawExplosion(SDL2pp::Renderer& renderer, const GameCamera& camera) {
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        for (int i = 0; i < 10; ++i) {
            explotion_animation.update();
            explotion_animation.draw(renderer, screenX, screenY, BOMB_WIDTH, BOMB_HEIGHT, 0.0,
                                     SDL_Point{16, 16});
        }
    }

    void activate_bomb() { active = true; }

    void explode() { exploding = true; }

    void reset() {
        exploding = false;
        active = false;
    }

    bool is_exploding() const { return exploding; }

    bool is_active() const { return active; }
};

#endif
