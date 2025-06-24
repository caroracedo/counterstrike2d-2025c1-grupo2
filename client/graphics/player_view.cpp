#include "player_view.h"

PlayerView::PlayerView(TextureManager& texture_manager, SoundManager& sound_manager, uint16_t id,
                       PlayerSkin skin):
        texture_manager(texture_manager),
        sound_manager(sound_manager),
        legs_view(*texture_manager.get_texture("legs"), 100),
        gun_view(texture_manager, sound_manager),
        id(id),
        skin(skin) {
    initialize_resources();
}

void PlayerView::initialize_resources() {
    player_skins[PlayerSkin::SEALFORCE] = "ct1";
    player_skins[PlayerSkin::GERMANGSG9] = "ct2";
    player_skins[PlayerSkin::UKSAS] = "ct3";
    player_skins[PlayerSkin::FRENCHGIGN] = "ct4";
    player_skins[PlayerSkin::PHEONIX] = "t1";
    player_skins[PlayerSkin::L337KREW] = "t2";
    player_skins[PlayerSkin::ARTICAVENGER] = "t3";
    player_skins[PlayerSkin::GUERRILLA] = "t4";
}

bool PlayerView::update_position(float x, float y, uint16_t new_life) {
    const float epsilon = 0.01f;

    if (new_life < life) {
        sound_manager.play("hit");
    }

    this->life = new_life;


    if (std::abs(posX - x) < epsilon && std::abs(posY - y) < epsilon) {
        return false;
    }

    posX = x;
    posY = y;
    return true;
}

void PlayerView::update_styles(PlayerType new_type, WeaponModel new_posture, PlayerSkin new_skin) {

    if (new_skin != skin) {
        skin = new_skin;
    }

    type = new_type;
    if (new_posture == WeaponModel::GLOCK) {
        current_frame = SDL2pp::Rect{32, 32, 32, 32};
    } else if (new_posture == WeaponModel::KNIFE) {
        current_frame = SDL2pp::Rect{0, 0, 32, 32};
    } else {
        current_frame = SDL2pp::Rect{0, 64, 32, 32};
    }
}

void PlayerView::draw(SDL2pp::Renderer& renderer, const GameCamera& camera) {
    if (!camera.is_visible(posX, posY, PLAYER_WIDTH, PLAYER_HEIGHT))
        return;

    float screenX = posX - camera.get_x();
    float screenY = posY - camera.get_y();
    float draw_angle = angle;


    for (const auto& effect: active_effects) {
        auto [dx, dy] = effect->get_offset(angle);
        screenX += dx;
        screenY += dy;
        draw_angle += effect->get_rotation_offset();

        legs_view.update(posX + dx, posY + dy);
        gun_view.update(posX + dx, posY + dy, gun_view.get_current_type());

        auto gun_tip = gun_view.get_gun_tip_screen_position(camera, draw_angle);

        effect->draw(renderer, gun_tip.first, gun_tip.second, draw_angle);
    }

    SDL2pp::Texture& texture = *texture_manager.get_texture(player_skins[skin]);

    SDL2pp::Rect dstRect{static_cast<int>(screenX) - PLAYER_WIDTH / 2,
                         static_cast<int>(screenY) - PLAYER_HEIGHT / 2, PLAYER_WIDTH,
                         PLAYER_HEIGHT};

    SDL_Point center{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2};


    legs_view.draw(renderer, camera, draw_angle);
    renderer.Copy(texture, current_frame, dstRect, draw_angle, center, SDL_FLIP_NONE);
    gun_view.draw(renderer, camera, draw_angle);

    for (auto& effect: active_effects) {
        effect->update();
    }

    active_effects.erase(std::remove_if(active_effects.begin(), active_effects.end(),
                                        [](const auto& eff) { return !eff->is_active(); }),
                         active_effects.end());
}


float PlayerView::get_x() const { return posX; }

float PlayerView::get_y() const { return posY; }

float PlayerView::get_angle() const { return angle; }

void PlayerView::update_angle(float angle) { this->angle = 360.0f - (angle - 90.0f); }

void PlayerView::start_knife_animation() {
    SDL2pp::Texture& texture = *texture_manager.get_texture("slash");
    active_effects.emplace_back(std::make_unique<KnifeSwingEffect>(texture));
}

void PlayerView::start_kickback() {
    SDL2pp::Texture& texture = *texture_manager.get_texture("muzzle_flash");
    active_effects.emplace_back(std::make_unique<KickbackEffect>(texture));
}


bool PlayerView::update(const ObjectDTO& object) {
    float new_angle = object.angle;
    float x = static_cast<float>(object.position[0]);
    float y = static_cast<float>(object.position[1]);

    bool moved = update_position(x, y, object.health);

    update_angle(new_angle);
    if (moved) {
        legs_view.update(x, y);
    }

    update_styles(object.player_type, object.weapon_model, object.player_skin);
    gun_view.update(x, y, object.weapon_model);

    return moved;
}


WeaponModel PlayerView::get_current_weapon() const { return gun_view.get_current_type(); }

bool PlayerView::can_attack() {
    uint32_t current_time = SDL_GetTicks();

    uint32_t cd = gun_view.get_weapon_cooldown(get_current_weapon());

    if (current_time - last_attack_time < cd) {
        return false;
    }
    last_attack_time = current_time;
    return true;
}
