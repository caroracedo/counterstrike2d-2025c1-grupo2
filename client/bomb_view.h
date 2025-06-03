#ifndef BOMB_VIEW_H
#define BOMB_VIEW_H

#include <SDL2pp/SDL2pp.hh>

#include "../common/constants.h"

#include "game_camera.h"
#include "animation.h"

#define GUN_WIDTH 32
#define GUN_HEIGHT 32

using namespace SDL2pp;

class BombView {
private:
    Animation explotion_animation;
    Texture& texture;
    float x, y;
    bool exploding = false;
    bool active = false;


public:
    
    BombView(Texture& tex, Texture& explotion): 
        explotion_animation(explotion,std::vector<SDL2pp::Rect>{
            Rect(0, 0, 32, 32), Rect(32, 0, 32, 32),
            Rect(64, 0, 32, 32), Rect(96, 0, 32, 32),
            Rect(128, 0, 32, 32), Rect(0, 32, 32, 32),
            Rect(32, 32, 32, 32), Rect(64, 32, 32, 32),
            Rect(96, 32, 32, 32), Rect(128, 32, 32, 32),
            Rect(0, 0, 32, 32), Rect(32, 0, 32, 32),
            Rect(0, 64, 32, 32), Rect(32, 64, 32, 32),
            Rect(32, 0, 32, 32), Rect(64, 0, 32, 32),
            Rect(64, 64, 32, 32), Rect(96, 64, 32, 32),
            Rect(128, 64, 32, 32), Rect(0, 96, 32, 32),
            Rect(32, 96, 32, 32), Rect(64, 96, 32, 32),
            Rect(96, 96, 32, 32), Rect(128, 96, 32, 32), 
            Rect(0, 128, 32, 32), Rect(32, 128, 32, 32), 
            Rect(64, 128, 32, 32)},100),
        texture(tex) {}

    void update(float px, float py) {
        x = px;
        y = py;
    }


    // ...existing code...

    void draw(Renderer& renderer, GameCamera& camera) {
        // Dibuja la bomba en la posición ajustada por la cámara
        SDL_Rect dst_rect = { int(x) - camera.get_x() + (GUN_WIDTH / 2), int(y) - camera.get_y() + (GUN_HEIGHT / 2),GUN_WIDTH, GUN_HEIGHT};
        SDL_Rect src_rect = { 0, 0, GUN_WIDTH, GUN_HEIGHT}; // O el tamaño real de la textura
     
        renderer.Copy(texture, src_rect, dst_rect);
     
    }

    void drawExplosion(Renderer& renderer, GameCamera& camera) {
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        for (int i = 0; i < 10; ++i) {
            explotion_animation.update();
            explotion_animation.draw(renderer, screenX, screenY, GUN_WIDTH, GUN_HEIGHT, 0.0, SDL_Point{16, 16});
        }
    }

    void activate_bomb() {
        active = true; 
    }

    void explode() {
        exploding = true;
    }

    void reset() {
        exploding = false;
        active = false;
    }

    bool is_exploding() const {
        return exploding;
    }
    bool is_active() const {
        return active ;
    }


};

#endif
