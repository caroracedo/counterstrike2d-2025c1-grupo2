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
    uint16_t ammo;

    /* Obstáculo */
    uint16_t width;
    uint16_t height;

    /* Bomba */
    uint16_t bomb_countdown;

    /* Player */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id,
              PlayerType player_type, WeaponModel weapon_model, uint16_t health, uint16_t money,
              uint16_t ammo):
            type(type),
            position(position),
            id(id),
            player_type(player_type),
            weapon_model(weapon_model),
            health(health),
            money(money),
            ammo(ammo),
            width(),
            height(),
            bomb_countdown() {}

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
            ammo(),
            width(width),
            height(height),
            bomb_countdown() {}

    /* Bala */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position):
            type(type),
            position(position),
            id(),
            player_type(),
            weapon_model(),
            health(),
            money(),
            ammo(),
            width(),
            height(),
            bomb_countdown() {}

    /* Bomba */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t bomb_countdown):
            type(type),
            position(position),
            id(),
            player_type(),
            weapon_model(),
            health(),
            money(),
            ammo(),
            width(),
            height(),
            bomb_countdown(bomb_countdown) {}
};

#endif  // OBJECT_DTO_H
