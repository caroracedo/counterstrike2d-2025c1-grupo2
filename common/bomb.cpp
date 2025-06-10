#include "bomb.h"

Bomb::Bomb(const std::vector<uint16_t>& position):
        Object(ObjectType::BOMB, 0, position, BOMB_RADIUS, BOMB_RADIUS) {}

ObjectDTO Bomb::get_dto() const {
    return ObjectDTO(object_type, position, std::floor(bomb_countdown / 30));
}

void Bomb::start_countdown() { bomb_countdown = BOMB_COUNTDOWN; }

bool Bomb::update_countdown() { return bomb_countdown >= 0 && --bomb_countdown == 0; }
