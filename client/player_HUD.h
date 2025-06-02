#ifndef PLAYER_HUD_H
#define PLAYER_HUD_H

#include <SDL2pp/SDL2pp.hh>
#include <string>

#include "../common/object_DTO.h"
#include "../common/constants.h"

class PLayerHUD {
private:
    SDL2pp::Texture& texture;
    SDL2pp::Renderer& renderer;

    uint16_t health;
    uint16_t money;
    uint16_t bullets;

    // cambiar el static
    static constexpr int digit_width = 48;
    static constexpr int digit_height = 66;

    void render_number_string(const std::string& text, int x_pos, int y_pos, float scale = 1.0f) {
        for (char c : text) {
            if (c >= '0' && c <= '9') {
                int index = c - '0';
                SDL2pp::Rect src(index * digit_width, 0, digit_width, digit_height);
                SDL2pp::Rect dst(x_pos, y_pos, digit_width * scale, digit_height * scale);
                texture.SetColorMod(255, 255, 0); // amarillo
                renderer.Copy(texture, src, dst);
                x_pos += digit_width * scale;
            }
        }
    }



public:
    PLayerHUD(SDL2pp::Texture& texture, SDL2pp::Renderer& renderer)
        : texture(texture), renderer(renderer), health(0), money(0), bullets(0) {}

    void update(const ObjectDTO& object) {
        health = object.health;
        money = object.money;
        bullets = 200; 
    }

    void draw() {         
        std::string health_str = std::to_string(health);
        std::string money_str = std::to_string(money);
        std::string bullets_str = std::to_string(bullets);
        float scale = 0.5f;

        render_number_string(health_str, 0, SCREEN_HEIGHT - digit_height * scale, scale);

        int bullet_x = SCREEN_WIDTH - digit_width * bullets_str.size() * scale;
        int bullet_y = SCREEN_HEIGHT - digit_height * scale;
        render_number_string(bullets_str, bullet_x, bullet_y, scale);

        // DINERO - Encima de las balas
        int money_x = SCREEN_WIDTH - digit_width * money_str.size() * scale;
        int money_y = bullet_y - digit_height * scale - 5;
        render_number_string(money_str, money_x, money_y, scale);
    }
    
};

#endif  // PLAYER_HUD_H
