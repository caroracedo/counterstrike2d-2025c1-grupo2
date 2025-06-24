#include "kickback.h"

#include <cmath>

#include <SDL2/SDL.h>

KickbackEffect::KickbackEffect(SDL2pp::Texture& texture, uint32_t duration_ms):
        muzzle_flash(texture) {
    duration = duration_ms;
    start_time = SDL_GetTicks();
}

void KickbackEffect::update() {
    if (SDL_GetTicks() - start_time > duration)
        active = false;
}

bool KickbackEffect::is_active() const { return active; }

std::pair<float, float> KickbackEffect::get_offset(float angle_deg) const {
    float rad = angle_deg * M_PI / 180.0f;
    return {-KICKBACK_DISTANCE * std::cos(rad), -KICKBACK_DISTANCE * std::sin(rad)};
}

void KickbackEffect::draw(SDL2pp::Renderer& renderer, float x, float y, float angle) {
    (void)renderer;
    (void)x;
    (void)y;
    (void)angle;
}
