#ifndef KNIFE_SWING_H
#define KNIFE_SWING_H

#include "visual_effect.h"

class KnifeSwingEffect: public VisualEffect {
private:
    SDL2pp::Texture& swing;

public:
    explicit KnifeSwingEffect(SDL2pp::Texture& texture, uint32_t duration_ms = 150):
            swing(texture) {
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

    void draw(SDL2pp::Renderer& renderer, float x, float y, float angle) override {
        if (!active)
            return;


        SDL2pp::Rect dst_rect = {static_cast<int>(x - 32), static_cast<int>(y - 32), 32, 32};

        SDL_Rect src_rect = {0, 0, 32, 32};
        swing.SetColorMod(255, 255, 255);
        renderer.Copy(swing, src_rect, dst_rect, angle, SDL_Point{16, 16}, SDL_FLIP_NONE);
    }
};

#endif  // KNIFE_SWING_H
