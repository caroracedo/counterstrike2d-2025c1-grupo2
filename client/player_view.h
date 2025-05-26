#ifndef PLAYER_VIEW_H
#define PLAYER_VIEW_H

#include <SDL2pp/SDL2pp.hh>
#include <cmath>
#include "../common/constants.h"
#include "game_camera.h"

using namespace SDL2pp;

class PlayerView {
private:
    float posX = 0.0f, posY = 0.0f;
    Texture& cuerpo;
    float angle = 0.0f; // Angle in degrees

public:
    PlayerView(Texture& cuerpo) : cuerpo(cuerpo) {}

    void update_position(float x, float y) {
        posX = x;
        posY = y;
    }

    float get_x() const {
        return posX;
    }
    float get_y() const {
        return posY;
    }
    
    void draw(Renderer& renderer, GameCamera& camera) {
        float screenX = posX - camera.get_x();
        float screenY = posY - camera.get_y();

        float centerX = screenX + PLAYER_WIDTH / 2.0f;
        float centerY = screenY + PLAYER_HEIGHT / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        float dx = mouseX - centerX;
        float dy = mouseY - centerY;

        float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;
        this->angle = angle;
        renderer.Copy(
            cuerpo,
            Rect(0, 0, PLAYER_HEIGHT, PLAYER_WIDTH),
            Rect(static_cast<int>(screenX), static_cast<int>(screenY), PLAYER_WIDTH, PLAYER_HEIGHT),
            angle,
            SDL_Point{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2},
            SDL_FLIP_NONE
        );
    }
    float get_angle() const {
        return angle;
    }

};

#endif
