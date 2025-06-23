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

public:
    DropView(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            renderer(renderer), texture_manager(texture_manager) {
        initialize_resources();
    }

    void initialize_resources() {
        drops[WeaponModel::GLOCK] = "glock_d";
        drops[WeaponModel::AK47] = "ak47_d";
        drops[WeaponModel::AWP] = "awp_d";
        drops[WeaponModel::M3] = "m3_d";

        sizes[WeaponModel::AK47] = {51, 17};
        sizes[WeaponModel::M3] = {50, 16};
        sizes[WeaponModel::AWP] = {66, 16};
    }

    void update(float x, float y, WeaponModel type) {
        this->type = type;
        this->x = x;
        this->y = y;
    }

    void draw(const GameCamera& camera) {
        if (type == WeaponModel::UNKNOWN)
            return;

        if (!camera.is_visible(x, y, sizes[type].width, sizes[type].height))
            return;

        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        int width = sizes[type].width;
        int height = sizes[type].height;

        SDL_Rect src_rect = {0, 0, width, height};

        float scale = width / GUN_WIDTH;

        float scaled_height = std::floor(height / scale);

        SDL2pp::Rect dst_rect = {static_cast<int>(screenX), static_cast<int>(screenY), GUN_WIDTH,
                                 static_cast<int>(scaled_height)};

        SDL2pp::Texture& texture = *texture_manager.get_texture(drops[type]);

        renderer.Copy(texture, src_rect, dst_rect);
    }
};

#endif  // DROP_VIEW_H
