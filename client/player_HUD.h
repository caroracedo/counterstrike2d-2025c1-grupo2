#ifndef PLAYER_HUD_H
#define PLAYER_HUD_H

#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../common/constants.h"
#include "../common/object_DTO.h"

// Agregar el 💲 y ❤️
class PLayerHUD {
private:
    SDL2pp::Texture& texture;
    SDL2pp::Texture vida;
    SDL2pp::Texture plata;
    SDL2pp::Renderer& renderer;


    uint16_t health;
    uint16_t money;
    uint16_t bullets;
    int timer = -1;

    // cambiar el static
    static constexpr int digit_width = 48;
    static constexpr int digit_height = 66;

    void render_number_string(const std::string& text, int x_pos, int y_pos, float scale,
                              bool yellow) {
        for (char c: text) {
            if (c >= '0' && c <= '9') {
                int index = c - '0';
                SDL2pp::Rect src(index * digit_width, 0, digit_width, digit_height);
                SDL2pp::Rect dst(x_pos, y_pos, digit_width * scale, digit_height * scale);
                if (yellow) {
                    texture.SetColorMod(255, 255, 0);  // amarillo
                } else {
                    texture.SetColorMod(255, 0, 0);  // rojo
                }
                renderer.Copy(texture, src, dst);
                x_pos += digit_width * scale;
            }
        }
    }

    void render_icon(int x_pos, int y_pos, float scale, bool vid) {
        if (vid) {
            SDL2pp::Rect src(0, 0, 64, 64);
            SDL2pp::Rect dst(x_pos, y_pos, 64 * scale, 64 * scale);
            texture.SetColorMod(255, 255, 0);  // amarillo
            renderer.Copy(vida, src, dst);
        } else {
            SDL2pp::Rect src(0, 0, 64, 64);
            SDL2pp::Rect dst(x_pos, y_pos, 64 * scale, 64 * scale);
            texture.SetColorMod(255, 255, 0);  // amarillo
            renderer.Copy(plata, src, dst);
        }
    }


public:
    PLayerHUD(SDL2pp::Texture& texture, SDL2pp::Renderer& renderer):
            texture(texture),
            vida(renderer, SDL2pp::Surface(SDL_LoadBMP("../sprite_vida.bmp")).SetColorKey(true, 0)),
            plata(renderer,
                  SDL2pp::Surface(SDL_LoadBMP("../sprite_plata.bmp")).SetColorKey(true, 0)),
            renderer(renderer),
            health(0),
            money(0),
            bullets(0) {}

    void update(const ObjectDTO& object) {
        health = object.health;
        money = object.money;
        bullets = 200;
    }

    void update_timer(const ObjectDTO& object) { timer = object.bomb_countdown; }

    void draw() {
        std::string health_str = std::to_string(health);
        std::string money_str = std::to_string(money);
        std::string bullets_str = std::to_string(bullets);
        std::string timer_str = std::to_string(timer);
        float scale = 0.5f;

        int health_x = 0;
        int health_y = SCREEN_HEIGHT - digit_height * scale;

        render_icon(health_x, health_y, scale, true);
        render_number_string(health_str, health_x + digit_width * scale, health_y, scale, true);

        int bullet_x = SCREEN_WIDTH - digit_width * bullets_str.size() * scale;
        int bullet_y = SCREEN_HEIGHT - digit_height * scale;

        render_number_string(bullets_str, bullet_x, bullet_y, scale, true);

        int money_x = SCREEN_WIDTH - digit_width * (money_str.size() + 1) * scale;
        int money_y = bullet_y - digit_height * scale - 5;
        render_icon(money_x, money_y, scale, false);  // 1 = signo pesos
        render_number_string(money_str, money_x + digit_width * scale, money_y, scale, true);

        int timer_x = SCREEN_WIDTH / 2 - digit_width * timer_str.size() * scale;


        if (timer > 0) {
            render_number_string(timer_str, timer_x, 0, scale, false);
        }
    }
};

#endif  // PLAYER_HUD_H
