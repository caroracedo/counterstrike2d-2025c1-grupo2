#ifndef KNIFE_SWING_H
#define KNIFE_SWING_H

#include "visual_effect.h"

class KnifeSwingEffect: public VisualEffect {
private:
    SDL2pp::Texture& swing;

public:
    explicit KnifeSwingEffect(SDL2pp::Texture& texture, uint32_t duration_ms = 150);

    void update() override;
    bool is_active() const override;
    float get_rotation_offset() const override;
    void draw(SDL2pp::Renderer& renderer, float x, float y, float angle) override;
};

#endif  // KNIFE_SWING_H
