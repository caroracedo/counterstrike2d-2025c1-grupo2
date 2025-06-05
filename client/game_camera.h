#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <SDL2pp/Rect.hh>

class GameCamera {
private:
    float x, y;
    int screen_width, screen_height;
    float map_width, map_height;

public:
    GameCamera(float screen_w, float screen_h, float map_w, float map_h);

    void center_on(float target_x, float target_y);

    SDL2pp::Point world_to_screen(float worldX, float worldY) {
        return SDL2pp::Point(worldX - x, worldY - y);
    }

    SDL2pp::Rect get_viewport() const;

    float get_x() const { return x; }
    float get_y() const { return y; }
};

#endif
