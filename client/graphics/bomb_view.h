#ifndef BOMB_VIEW_H
#define BOMB_VIEW_H

#include <utility>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"

#include "animation.h"
#include "game_camera.h"
#include "sound_manager.h"

#define BOMB_WIDTH 32
#define BOMB_HEIGHT 32

class BombView {
private:
    Animation explotion_animation;

    SDL2pp::Texture& bomb_texture;

    float x, y;

    bool exploding = false;

    bool active = false;

    bool dropped = false;

    bool sounds_played = false;

    SoundManager& sounds;

    std::vector<SDL2pp::Rect> get_rects();

public:
    BombView(SDL2pp::Texture& bomb_texture, SDL2pp::Texture& explotion, SoundManager& sm);

    void update(float px, float py, uint16_t time_to_explode,
                std::optional<std::pair<float, float>> listener_position = std::nullopt);

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera);

    void drawExplosion(SDL2pp::Renderer& renderer, const GameCamera& camera);

    void activate_bomb();

    void explode();

    void reset();

    bool is_exploding() const;

    bool is_active() const;

    void reset_sounds() { sounds_played = false; }

    bool is_dropped() const { return dropped; }

    void set_dropped(bool dropped) { this->dropped = dropped; }
};

#endif
