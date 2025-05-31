#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <vector>

#include "types.h"

struct ObjectDTO {
    ObjectType type;
    std::vector<uint16_t> position;

    /* Player */
    uint16_t id;
    PlayerType player_type;
    WeaponModel weapon_model;
    uint16_t health;
    uint16_t money;

    /* Obstáculo */
    std::uint16_t width;
    std::uint16_t height;

    /* Player */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id,
              PlayerType player_type, WeaponModel weapon_model, uint16_t health, uint16_t money):
            type(type),
            position(position),
            id(id),
            player_type(player_type),
            weapon_model(weapon_model),
            health(health),
            money(money),
            width(),
            height() {}

    /* Obstáculo */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
              uint16_t height):
            type(type),
            position(position),
            id(),
            player_type(),
            weapon_model(),
            health(),
            money(),
            width(width),
            height(height) {}

    /* Bala */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position):
            type(type),
            position(position),
            id(),
            player_type(),
            weapon_model(),
            health(),
            money(),
            width(),
            height() {}
};

#endif  // OBJECT_DTO_H
