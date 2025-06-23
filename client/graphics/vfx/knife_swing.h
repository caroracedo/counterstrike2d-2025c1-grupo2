#ifndef KNIFE_SWING_H
#define KNIFE_SWING_H

#include "visual_effect.h"

class KnifeSwingEffect: public VisualEffect {
public:
    explicit KnifeSwingEffect(uint32_t duration_ms = 150) {
        start_time = SDL_GetTicks();
        duration = duration_ms;
    }


    void update() override {
        if (SDL_GetTicks() - start_time > duration)
            active = false;
    }

    bool is_active() const override { return active; }

    float get_rotation_offset() const override {
        uint32_t elapsed = SDL_GetTicks() - start_time;
        if (elapsed < duration) {
            return std::sin(elapsed * 0.06f) * 20.0f;
        }
        return 0.0f;
    }
};

#endif  // KNIFE_SWING_H
