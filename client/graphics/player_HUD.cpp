#include "player_HUD.h"

#include <limits>

PlayerHUD::PlayerHUD(SDL2pp::Renderer& renderer, SDL2pp::Texture& numbers_texture,
                     SDL2pp::Texture& life_texture, SDL2pp::Texture& money_texture):
        renderer(renderer),
        numbers(numbers_texture),
        life_texture(life_texture),
        money_texture(money_texture) {}

void PlayerHUD::render_number_string(const std::string& text, int x_pos, int y_pos, float scale,
                                     bool yellow) {
    for (char c: text) {
        if (c >= '0' && c <= '9') {
            int index = c - '0';
            SDL2pp::Rect src(index * NUMBER_WIDTH, 0, NUMBER_WIDTH, NUMBER_HEIGHT);
            SDL2pp::Rect dst(x_pos, y_pos, NUMBER_WIDTH * scale, NUMBER_HEIGHT * scale);
            if (yellow) {
                numbers.SetColorMod(255, 255, 0);  // amarillo
            } else {
                numbers.SetColorMod(255, 0, 0);  // rojo
            }
            numbers.SetAlphaMod(127);  // opaco
            renderer.Copy(numbers, src, dst);
            x_pos += NUMBER_WIDTH * scale;
        }
    }
}

void PlayerHUD::render_icon(int x_pos, int y_pos, float scale, bool show_life) {
    if (show_life) {
        SDL2pp::Rect src(0, 0, 64, 64);
        SDL2pp::Rect dst(x_pos, y_pos, 64 * scale, 64 * scale);
        life_texture.SetAlphaMod(127);
        life_texture.SetColorMod(255, 255, 0);  // amarillo
        renderer.Copy(life_texture, src, dst);
    } else {
        SDL2pp::Rect src(0, 0, 64, 64);
        SDL2pp::Rect dst(x_pos, y_pos, 64 * scale, 64 * scale);
        money_texture.SetColorMod(255, 255, 0);  // amarillo
        money_texture.SetAlphaMod(127);
        renderer.Copy(money_texture, src, dst);
    }
}

void PlayerHUD::update(const ObjectDTO& object) {
    health = object.health;
    money = object.money;
    bullets = object.ammo;
}

void PlayerHUD::update_timer(const ObjectDTO& object) { timer = object.bomb_countdown; }

void PlayerHUD::draw(bool is_bomb_active) {
    std::string health_str = std::to_string(health);
    std::string money_str = std::to_string(money);
    std::string bullets_str = std::to_string(bullets);
    std::string timer_str = std::to_string(timer);
    float scale = 0.5f;

    // render health
    int health_x = 0;
    int health_y = SCREEN_HEIGHT - digit_height * scale;
    render_icon(health_x, health_y, scale, true);
    render_number_string(health_str, health_x + digit_width * scale, health_y, scale, true);

    // render bullets
    int bullet_x = SCREEN_WIDTH - digit_width * bullets_str.size() * scale;
    int bullet_y = SCREEN_HEIGHT - digit_height * scale;
    render_number_string(bullets_str, bullet_x, bullet_y, scale, true);

    // render money
    int money_x = SCREEN_WIDTH - digit_width * (money_str.size() + 1) * scale;
    int money_y = bullet_y - digit_height * scale - 5;
    render_icon(money_x, money_y, scale, false);
    render_number_string(money_str, money_x + digit_width * scale, money_y, scale, true);

    // when bomb planted, render time remaining
    int timer_x = SCREEN_WIDTH / 2 - digit_width * timer_str.size() * scale;

    if (timer > 0 && is_bomb_active && timer != std::numeric_limits<uint16_t>::max()) {
        render_number_string(timer_str, timer_x, 0, scale, false);
    }
}
