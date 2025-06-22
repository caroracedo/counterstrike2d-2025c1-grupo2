#ifndef OBJECT_DTO_H
#define OBJECT_DTO_H

#include <cstdint>
#include <string>
#include <vector>

#include "types.h"
#include "weapon_DTO.h"

struct ObjectDTO {
    ObjectType type;
    std::vector<uint16_t> position;

    /* Player */
    uint16_t id;
    PlayerType player_type;
    PlayerSkin player_skin;
    float angle;

    /* Arma */
    WeaponModel weapon_model;
    uint16_t health;
    uint16_t money;
    uint16_t ammo;

    /* Obstáculo */
    uint16_t width;
    uint16_t height;
    ObstacleType obstacle_type;

    /* Bomba */
    uint16_t bomb_countdown;

    /* Unknown */
    ObjectDTO();

    /* Player */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id,
              PlayerType player_type, PlayerSkin player_skin, WeaponModel weapon_model,
              uint16_t health, uint16_t money, uint16_t ammo, float angle);

    /* Obstáculo */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
              uint16_t height, ObstacleType obstacle_type);

    /* Zona de Bomba */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
              uint16_t height);

    /* Bala */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position);

    /* Bomba */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t bomb_countdown);

    /* Arma */
    ObjectDTO(ObjectType type, const std::vector<uint16_t>& position,
              const WeaponModel& weapon_model);

    /* Tests */
    bool operator==(const ObjectDTO& other) const {
        return type == other.type && position == other.position && id == other.id &&
               player_type == other.player_type && angle == other.angle &&
               weapon_model == other.weapon_model && health == other.health &&
               money == other.money && ammo == other.ammo && width == other.width &&
               height == other.height && obstacle_type == other.obstacle_type &&
               bomb_countdown == other.bomb_countdown;
    }
};

#endif  // OBJECT_DTO_H
