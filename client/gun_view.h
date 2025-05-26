#ifndef GUN_VIEW_H
#define GUN_VIEW_H

#include <cmath>
#include <SDL2pp/SDL2pp.hh>
#include "game_camera.h"
#include "../common/constants.h"

using namespace SDL2pp;

class GunView {
private:
    Texture& texture;
    int offset_x = 10;
    int offset_y = 10;

public:
    GunView(Texture& tex) : texture(tex) {}

    void draw(Renderer& renderer, const GameCamera& camera, float player_x, float player_y, float angle_deg) {
        // float angle_rad = angle_deg * M_PI / 180.0f;

        // Punto donde queremos anclar el arma (mano del jugador)
        int hand_x = static_cast<int>(player_x + PLAYER_WIDTH / 2);
        int hand_y = static_cast<int>(player_y + PLAYER_HEIGHT / 2);

        // Elegimos como punto de rotación el mango del arma (por ejemplo: x = 6px desde el borde izquierdo)
        SDL_Point pivot = {18, texture.GetHeight() / 2};

        // El arma se dibuja en pantalla con su PIVOT alineado al punto de la mano
        int draw_x = hand_x - pivot.x - camera.get_x();
        int draw_y = hand_y - pivot.y - camera.get_y();

        renderer.Copy(
            texture,
            Rect(0, 0, texture.GetWidth(), texture.GetHeight()),
            Rect(draw_x, draw_y, texture.GetWidth(), texture.GetHeight()),
            angle_deg,
            pivot,
            SDL_FLIP_NONE
        );
    }



};

#endif
