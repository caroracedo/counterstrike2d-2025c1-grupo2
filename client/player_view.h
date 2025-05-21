#ifndef PLAYER_VIEW_H
#define PLAYER_VIEW_H

#include <SDL2pp/SDL2pp.hh>
#include <cmath>
#include "../common/constants.h"

using namespace SDL2pp;

class PlayerView {
private:
    float posX = 0.0f, posY = 0.0f;
    Texture& cuerpo;

public:
    PlayerView(Texture& cuerpo) : cuerpo(cuerpo) {}

    void update_position(float x, float y) {
        posX = x;
        posY = y;
    }

    void draw(Renderer& renderer) {
        float centerX = posX + PLAYER_WIDTH / 2.0f;
        float centerY = posY + PLAYER_HEIGHT / 2.0f;

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float dx = mouseX - centerX;
        float dy = mouseY - centerY;
        float angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;

        renderer.Copy(
            cuerpo,
            Rect(0, 0, PLAYER_HEIGHT, PLAYER_WIDTH),
            Rect(static_cast<int>(posX), static_cast<int>(posY), PLAYER_WIDTH, PLAYER_HEIGHT),
            angle,
            SDL_Point{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2},
            SDL_FLIP_NONE
        );
    }
};

#endif
