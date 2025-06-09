#include "bomb_zone.h"

BombZone::BombZone(const std::vector<uint16_t>& position, uint16_t width, uint16_t height):
            Object(ObjectType::BOMBZONE, 0, position, width, height) {}

ObjectDTO BombZone::get_dto() const { return ObjectDTO(object_type, position, width, height); }