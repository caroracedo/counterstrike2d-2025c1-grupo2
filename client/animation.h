#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2pp/SDL2pp.hh>
#include <vector>

using namespace SDL2pp;

class Animation {
private:
    Texture& texture;            
    std::vector<Rect> frames;    
    int current_frame = 0;
    Uint32 frame_duration;         
    Uint32 last_update_time = 0;   

public:
    Animation(Texture& texture, std::vector<Rect> frames, Uint32 frame_duration_ms)
        : texture(texture), frames(std::move(frames)), frame_duration(frame_duration_ms), last_update_time(SDL_GetTicks()) {}

    void update() {
        Uint32 now = SDL_GetTicks();
        if (now - last_update_time >= frame_duration) {
            current_frame = (current_frame + 1) % frames.size();
            last_update_time = now;
        }
    }

    void draw(Renderer& renderer, int x, int y, int w, int h,double angle, SDL_Point center) {
        renderer.Copy(
            texture,
            frames[current_frame],
            Rect(x, y, w, h),
            angle,
            center, 
            SDL_FLIP_NONE
        );
    }

    void reset() {
        current_frame = 0;
        last_update_time = SDL_GetTicks();
    }
};

#endif
