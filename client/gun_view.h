#ifndef GUN_VIEW_H
#define GUN_VIEW_H

#include <cmath>
#include <SDL2pp/SDL2pp.hh>
#include "game_camera.h"
#include "../common/constants.h"

#define GUN_WIDTH 32
#define GUN_HEIGHT 32

using namespace SDL2pp;

class GunView {
private:
    Texture& texture;
    float x, y;
    float angle = 0.0f;  // Grados

public:
    GunView(Texture& tex) : texture(tex) {}

    void update(float px, float py) {
        x = px;
        y = py;
    }

    void update_angle(float new_angle) {
        angle = new_angle;
    }

    void draw(Renderer& renderer, GameCamera& camera) {
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        // // Calcular el centro de la pistola
        // float centerX = screenX + GUN_WIDTH / 2.0f;
        // float centerY = screenY + GUN_HEIGHT / 2.0f;

        // Dibujar la pistola con el ángulo calculado
        renderer.Copy(
            texture,
            Rect(0, 0, GUN_WIDTH, GUN_HEIGHT),
            Rect(static_cast<int>(screenX), static_cast<int>(screenY), GUN_WIDTH, GUN_HEIGHT),
            angle,
            SDL_Point{GUN_WIDTH/2, GUN_HEIGHT/2}, // Centro de la pistola
            SDL_FLIP_NONE
        );
    }



};

#endif
