#ifndef GUN_VIEW_H
#define GUN_VIEW_H

#include <cmath>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/types.h"

#include "game_camera.h"
#include "sound_manager.h"
#include "textures_manager.h"

class GunView {
private:
    TextureManager& texture_manager;
    SoundManager& sound_manager;
    WeaponModel current_type = WeaponModel::GLOCK;
    std::unordered_map<WeaponModel, std::string> weapons;
    float x = 0, y = 0;

public:
    explicit GunView(TextureManager& texture_manager, SoundManager& sound_manager);

    void update(float px, float py, WeaponModel type);

    void draw(SDL2pp::Renderer& renderer, const GameCamera& camera, float angle);

    std::pair<float, float> get_gun_tip_screen_position(const GameCamera& camera,
                                                        float angle) const;

    WeaponModel get_current_type() const;

    uint32_t get_weapon_cooldown(WeaponModel model);

    void play_weapon_sound() {
        auto sound_name =
                (current_type == WeaponModel::KNIFE) ? "knife_slash" : weapons[current_type];
        auto cooldown = get_weapon_cooldown(current_type);
        int loops = (current_type == WeaponModel::AK47) ? 3 : 0;
        sound_manager.playWithCooldown(sound_name, cooldown, loops);
    }
};

#endif
