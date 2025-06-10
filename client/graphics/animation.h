#ifndef ANIMATION_H
#define ANIMATION_H

#include <utility>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

class Animation {
private:
    SDL2pp::Texture& texture;
    std::vector<SDL2pp::Rect> frames;
    int current_frame = 0;
    uint32_t frame_duration;
    uint32_t last_update_time = 0;

public:
    Animation(SDL2pp::Texture& texture, std::vector<SDL2pp::Rect> frames,
              uint32_t frame_duration_ms):
            texture(texture),
            frames(std::move(frames)),
            frame_duration(frame_duration_ms),
            last_update_time(SDL_GetTicks()) {}

    void update() {
        uint32_t now = SDL_GetTicks();
        if (now - last_update_time >= frame_duration) {
            current_frame = (current_frame + 1) % frames.size();
            last_update_time = now;
        }
    }

    void draw(SDL2pp::Renderer& renderer, int x, int y, int w, int h, double angle,
              SDL_Point center) {
        renderer.Copy(texture, frames[current_frame], SDL2pp::Rect(x, y, w, h), angle, center,
                      SDL_FLIP_NONE);
    }

    void reset() {
        current_frame = 0;
        last_update_time = SDL_GetTicks();
    }
};

#endif
