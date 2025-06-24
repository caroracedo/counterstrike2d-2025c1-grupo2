#ifndef KICKBACK_EFFECT_H
#define KICKBACK_EFFECT_H

#include <utility>

#include "common/constants.h"

#include "visual_effect.h"

class KickbackEffect: public VisualEffect {
private:
    SDL2pp::Texture& muzzle_flash;

public:
    explicit KickbackEffect(SDL2pp::Texture& texture, uint32_t duration_ms = 100);

    void update() override;
    bool is_active() const override;
    std::pair<float, float> get_offset(float angle_deg) const override;
    void draw(SDL2pp::Renderer& renderer, float x, float y, float angle) override;
};

#endif  // KICKBACK_EFFECT_H
