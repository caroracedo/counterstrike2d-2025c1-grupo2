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
              uint32_t frame_duration_ms);
    void update();
    void draw(SDL2pp::Renderer& renderer, int x, int y, int w, int h, double angle,
              SDL_Point center);
    void reset();
};

#endif
