#ifndef KICKBACK_EFFECT_H
#define KICKBACK_EFFECT_H

#include <utility>

#include "common/constants.h"

#include "visual_effect.h"

class KickbackEffect: public VisualEffect {
private:
    SDL2pp::Texture& muzzle_flash;

public:
    explicit KickbackEffect(SDL2pp::Texture& texture, uint32_t duration_ms = 100):
            muzzle_flash(texture) {
        duration = duration_ms;
        start_time = SDL_GetTicks();
    }

    void update() override {
        if (SDL_GetTicks() - start_time > duration)
            active = false;
    }

    bool is_active() const override { return active; }

    std::pair<float, float> get_offset(float angle_deg) const override {
        float rad = angle_deg * M_PI / 180.0f;
        return {-KICKBACK_DISTANCE * std::cos(rad), -KICKBACK_DISTANCE * std::sin(rad)};
    }

    void draw(SDL2pp::Renderer& renderer, float x, float y, float angle) override {
        (void)renderer;
        (void)x;
        (void)y;
        (void)angle;
    }
};

#endif  // KICKBACK_EFFECT_H
