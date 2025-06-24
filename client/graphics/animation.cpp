#include "animation.h"

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

Animation::Animation(SDL2pp::Texture& texture, std::vector<SDL2pp::Rect> frames,
                     uint32_t frame_duration_ms):
        texture(texture),
        frames(std::move(frames)),
        current_frame(0),
        frame_duration(frame_duration_ms),
        last_update_time(SDL_GetTicks()) {}

void Animation::update() {
    uint32_t now = SDL_GetTicks();
    if (now - last_update_time >= frame_duration) {
        current_frame = (current_frame + 1) % frames.size();
        last_update_time = now;
    }
}

void Animation::draw(SDL2pp::Renderer& renderer, int x, int y, int w, int h, double angle,
                     SDL_Point center) {
    renderer.Copy(texture, frames[current_frame], SDL2pp::Rect(x, y, w, h), angle, center,
                  SDL_FLIP_NONE);
}

void Animation::reset() {
    current_frame = 0;
    last_update_time = SDL_GetTicks();
}
