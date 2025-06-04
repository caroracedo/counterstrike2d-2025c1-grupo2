#ifndef PLAYER_VIEW_H
#define PLAYER_VIEW_H

#include <cmath>

#include <SDL2pp/SDL2pp.hh>

#include "../common/constants.h"
#include "../common/types.h"

#include "game_camera.h"

class PlayerView {
private:
    float posX = 0.0f, posY = 0.0f;
    SDL2pp::Texture& cuerpo;
    float angle = 0.0f;

public:
    explicit PlayerView(SDL2pp::Texture& cuerpo): cuerpo(cuerpo) {}

    void update_position(float x, float y) {
        posX = x;
        posY = y;
    }

    float get_x() const { return posX; }
    float get_y() const { return posY; }

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
        float screenX = posX - camera.get_x();
        float screenY = posY - camera.get_y();

        float centerX = screenX + PLAYER_WIDTH / 2.0f;
        float centerY = screenY + PLAYER_HEIGHT / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        float dx = mouseX - centerX;
        float dy = mouseY - centerY;

        float new_angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;
        this->angle = new_angle;
        renderer.Copy(cuerpo,
                      SDL2pp::Rect(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, PLAYER_HEIGHT),
                      SDL2pp::Rect(static_cast<int>(screenX), static_cast<int>(screenY),
                                   PLAYER_WIDTH, PLAYER_HEIGHT),
                      angle, SDL_Point{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2}, SDL_FLIP_NONE);
    }
    float get_angle() const { return angle; }
};

#endif
