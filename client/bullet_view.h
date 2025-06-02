#ifndef BULLET_VIEW_H
#define BULLET_VIEW_H

#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include "game_camera.h"
#include "../common/action_DTO.h"

using namespace SDL2pp;

class BulletView {
private:
    // int id;
    float x;
    float y;
    // float angle; 
        
public:
        BulletView(float start_x, float start_y):  x(start_x), y(start_y){}
            
        void update(float new_x, float new_y) {
            x = new_x;
            y = new_y;
        }

        void draw(Renderer& renderer, const GameCamera& camera) {
            SDL_Rect rect = {
                static_cast<int>(x - camera.get_x()),
                static_cast<int>(y - camera.get_y()),
                4, 4
            };
            renderer.SetDrawColor(0, 0, 0, 1);  //dorado
            renderer.FillRect(rect);
        }
};

#endif
