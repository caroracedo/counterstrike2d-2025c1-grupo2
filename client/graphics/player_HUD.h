#ifndef PLAYER_HUD_H
#define PLAYER_HUD_H

#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/object_DTO.h"

#define NUMBER_WIDTH 48
#define NUMBER_HEIGHT 66

class PlayerHUD {
private:
    SDL2pp::Renderer& renderer;

    SDL2pp::Texture& numbers;
    SDL2pp::Texture& life_texture;
    SDL2pp::Texture& money_texture;


    uint16_t health = 0;
    uint16_t money = 0;
    uint16_t bullets = 0;
    uint16_t timer = -1;

    // cambiar el static
    static constexpr int digit_width = 48;
    static constexpr int digit_height = 66;

    void render_number_string(const std::string& text, int x_pos, int y_pos, float scale,
                              bool yellow);

    void render_icon(int x_pos, int y_pos, float scale, bool show_life);


public:
    PlayerHUD(SDL2pp::Renderer& renderer, SDL2pp::Texture& numbers, SDL2pp::Texture& life_texture,
              SDL2pp::Texture& money_texture);

    void update(const ObjectDTO& object);

    void update_timer(const ObjectDTO& object);

    void draw(bool is_bomb_active);
};

#endif  // PLAYER_HUD_H
