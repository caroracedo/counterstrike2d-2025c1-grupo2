#ifndef OBSTACLE_VIEW_H
#define OBSTACLE_VIEW_H

#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/types.h"

#include "game_camera.h"
#include "textures_manager.h"

class ObstacleView {
private:
    SDL2pp::Renderer& renderer;
    std::unordered_map<ObstacleType, std::string> boxes;

    ObstacleType type = ObstacleType::UNKNOWN;

    TextureManager& texture_manager;

    float x = 0, y = 0, w = 0, h = 0;

public:
    ObstacleView(SDL2pp::Renderer& renderer, TextureManager& textures):
            renderer(renderer), texture_manager(textures) {
        boxes[ObstacleType::OBSTACLE1] = "box_1";
        boxes[ObstacleType::OBSTACLE2] = "box_2";
        boxes[ObstacleType::OBSTACLE3] = "box_3";
        boxes[ObstacleType::OBSTACLE4] = "box_4";
        boxes[ObstacleType::OBSTACLE5] = "box_5";
    }

    void update(float x, float y, float w, float h, ObstacleType type) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;

        this->type = type;
    }

    void draw(const GameCamera& camera) {
        if (!camera.is_visible(x, y))
            return;
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        SDL2pp::Rect dst_rect = {static_cast<int>(screenX), static_cast<int>(screenY), int(w),
                                 int(h)};

        SDL2pp::Texture& texture = *texture_manager.get_texture(boxes[type]);
        SDL_Rect src_rect = {0, 0, int(w), int(h)};

        renderer.Copy(texture, src_rect, dst_rect);
    }
};

#endif
