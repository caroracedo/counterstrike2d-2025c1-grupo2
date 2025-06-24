#ifndef DROP_VIEW_H
#define DROP_VIEW_H

#include <cmath>
#include <map>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/types.h"

#include "game_camera.h"
#include "textures_manager.h"

struct Size {
    int width;
    int height;
};

class DropView {
private:
    SDL2pp::Renderer& renderer;
    std::unordered_map<WeaponModel, std::string> drops;
    WeaponModel type = WeaponModel::UNKNOWN;
    TextureManager& texture_manager;
    std::map<WeaponModel, Size> sizes;
    float x = 0, y = 0;

    void initialize_resources();

public:
    DropView(SDL2pp::Renderer& renderer, TextureManager& texture_manager);
    void update(float x, float y, WeaponModel type);
    void draw(const GameCamera& camera);
};

#endif  // DROP_VIEW_H
