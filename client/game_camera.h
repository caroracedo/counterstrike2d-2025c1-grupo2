#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <SDL2pp/Rect.hh>

using namespace SDL2pp;

class GameCamera {
private:
    int x, y;
    int screen_width, screen_height;
    int map_width, map_height;

public:
    GameCamera(int screen_w, int screen_h, int map_w, int map_h);

    void center_on(int target_x, int target_y);

    Rect get_viewport() const;

    int get_x() const { return x; }
    int get_y() const { return y; }
};

#endif
