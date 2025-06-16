#include "player_view.h"

PlayerView::PlayerView(TextureManager& texture_manager, uint16_t id):
        texture_manager(texture_manager), id(id) {
    player_types[PlayerType::TERRORIST] = "terrorists";
    player_types[PlayerType::COUNTERTERRORIST] = "counter_terrorists";
}

bool PlayerView::update_position(float x, float y) {
    const float epsilon = 0.01f;
    if (std::abs(posX - x) < epsilon && std::abs(posY - y) < epsilon) {
        return false;
    }

    posX = x;
    posY = y;
    return true;
}

// esta info va a venir de un .yaml
void PlayerView::update_styles(PlayerType new_type, WeaponModel new_posture) {
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
    if (!camera.is_visible(posX, posY))
        return;

    float screenX = posX - camera.get_x();
    float screenY = posY - camera.get_y();

    // float centerX = screenX + PLAYER_WIDTH / 2.0f;
    // float centerY = screenY + PLAYER_HEIGHT / 2.0f;

    // int mouseX, mouseY;
    // SDL_GetMouseState(&mouseX, &mouseY);

    // float dx = mouseX - centerX;
    // float dy = mouseY - centerY;

    // float new_angle = std::atan2(dy, dx) * 180.0f / M_PI + 90.0f;
    // this->angle = new_angle;

    SDL2pp::Texture& texture = *texture_manager.get_texture(player_types[type]);

    renderer.Copy(texture, current_frame,
                  SDL2pp::Rect(static_cast<int>(screenX) - PLAYER_WIDTH / 2,
                               static_cast<int>(screenY) - PLAYER_HEIGHT / 2, PLAYER_WIDTH,
                               PLAYER_HEIGHT),
                  angle, SDL_Point{PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2}, SDL_FLIP_NONE);
}

float PlayerView::get_x() const { return posX; }

float PlayerView::get_y() const { return posY; }

float PlayerView::get_angle() const { return angle; }
