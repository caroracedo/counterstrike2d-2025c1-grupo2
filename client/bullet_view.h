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
        float x, y;
        float vx, vy;
        float speed;
        double angle;
        bool active = true; 
        
    public:
        BulletView(float start_x, float start_y, float angle_deg):  x(start_x), y(start_y), angle(angle_deg){
            float angle_rad = angle_deg * M_PI / 180.0f;
            float speed = 5.0f;
            vx = std::cos(angle_rad) * speed;
            vy = std::sin(angle_rad) * speed;
        }
            
        void update() {
            if (!active) return;
            x += vx;
            y += vy;
            if (x < 0 || y < 0 || x > 2048 || y > 2048) active = false;
        }
        void draw(Renderer& renderer, const GameCamera& camera) {
            SDL_Rect rect = {
                static_cast<int>(x - camera.get_x()),
                static_cast<int>(y - camera.get_y()),
                5, 3
            };
            renderer.SetDrawColor(255, 255, 0, 255);  // Amarillo
            renderer.FillRect(rect);
        }
        bool is_active() const {
            return active;
        }
};

#endif
