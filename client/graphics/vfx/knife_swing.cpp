#include "knife_swing.h"

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>

KnifeSwingEffect::KnifeSwingEffect(SDL2pp::Texture& texture, uint32_t duration_ms): swing(texture) {
    start_time = SDL_GetTicks();
    duration = duration_ms;
}

void KnifeSwingEffect::update() {
    if (SDL_GetTicks() - start_time > duration)
        active = false;
}

bool KnifeSwingEffect::is_active() const { return active; }

float KnifeSwingEffect::get_rotation_offset() const {
    uint32_t elapsed = SDL_GetTicks() - start_time;
    if (elapsed < duration) {
        return std::sin(elapsed * 0.06f) * 20.0f;
    }
    return 0.0f;
}

void KnifeSwingEffect::draw(SDL2pp::Renderer& renderer, float x, float y, float angle) {
    if (!active)
        return;

    SDL2pp::Rect dst_rect = {static_cast<int>(x - 32), static_cast<int>(y - 32), 32, 32};
    SDL_Rect src_rect = {0, 0, 32, 32};
    swing.SetColorMod(255, 255, 255);
    renderer.Copy(swing, src_rect, dst_rect, angle, SDL_Point{16, 16}, SDL_FLIP_NONE);
}
