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
    ObstacleView(SDL2pp::Renderer& renderer, TextureManager& textures);
    void update(float x, float y, float w, float h, ObstacleType type);
    void draw(const GameCamera& camera);
};

#endif
