#ifndef DROP_VIEW_H
#define DROP_VIEW_H

#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/types.h"

#include "game_camera.h"
#include "textures_manager.h"

class DropView {
private:
    SDL2pp::Renderer& renderer;

    std::unordered_map<WeaponModel, std::string> drops;

    WeaponModel type = WeaponModel::UNKNOWN;

    TextureManager& texture_manager;

    float x = 0, y = 0;

public:
    DropView(SDL2pp::Renderer& renderer, TextureManager& texture_manager):
            renderer(renderer), texture_manager(texture_manager) {
        drops[WeaponModel::GLOCK] = "glock_d";
        drops[WeaponModel::AK47] = "ak47_d";
        drops[WeaponModel::AWP] = "awp_d";
        drops[WeaponModel::M3] = "m3_d";
    }

    void update(float x, float y, WeaponModel type) {
        this->type = type;
        this->x = x;
        this->y = y;
    }

    void draw(const GameCamera& camera) {
        if (type == WeaponModel::UNKNOWN)
            return;
        float screenX = x - camera.get_x();
        float screenY = y - camera.get_y();

        SDL2pp::Rect dst_rect = {static_cast<int>(screenX), static_cast<int>(screenY), 64,
                                 GUN_HEIGHT};

        SDL2pp::Texture& texture = *texture_manager.get_texture(drops[type]);
        SDL_Rect src_rect = {0, 0, 64, GUN_HEIGHT};
        std::cout << "Drawing drop at (" << screenX << ", " << screenY << ") with type "
                  << static_cast<int>(type) << std::endl;
        renderer.Copy(texture, src_rect, dst_rect);
    }
};

#endif  // DROP_VIEW_H
