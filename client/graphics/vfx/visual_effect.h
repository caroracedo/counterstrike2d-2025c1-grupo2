#ifndef VISUAL_EFFECT_H
#define VISUAL_EFFECT_H

#include <utility>

#include <SDL2pp/SDL2pp.hh>

class VisualEffect {
protected:
    uint32_t start_time;
    uint32_t duration;
    bool active = true;

public:
    virtual void update() = 0;
    virtual void draw(SDL2pp::Renderer& renderer, float x, float y, float angle) = 0;
    virtual bool is_active() const = 0;

    virtual std::pair<float, float> get_offset(float angle_deg) const {
        (void)angle_deg;
        return {0.0f, 0.0f};
    }

    virtual float get_rotation_offset() const { return 0.0f; }

    virtual ~VisualEffect();
};

#endif  // VISUAL_EFFECT_H
