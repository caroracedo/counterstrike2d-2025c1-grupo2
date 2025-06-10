#include "object_DTO.h"

/* Player */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id,
                     PlayerType player_type, WeaponModel weapon_model, uint16_t health,
                     uint16_t money, uint16_t ammo):
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
        obstacle_type(),
        bomb_countdown() {}

/* Obstáculo */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
                     uint16_t height, ObstacleType obstacle_type):
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
        obstacle_type(obstacle_type),
        bomb_countdown() {}

/* Zona de Bomba */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
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
        obstacle_type(),
        bomb_countdown() {}

/* Bala */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position):
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
        obstacle_type(),
        bomb_countdown() {}

/* Bomba */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position,
                     uint16_t bomb_countdown):
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
        obstacle_type(),
        bomb_countdown(bomb_countdown) {}
