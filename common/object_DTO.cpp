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
        width(0),
        height(0),
        obstacle_type(ObstacleType::UNKNOWN),
        bomb_countdown(0),
        range(0),
        min_damage(0),
        max_damage(0),
        precision(0.0f) {}

/* Obstáculo */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
                     uint16_t height, ObstacleType obstacle_type):
        type(type),
        position(position),
        id(0),
        player_type(PlayerType::UNKNOWN),
        weapon_model(WeaponModel::UNKNOWN),
        health(0),
        money(0),
        ammo(0),
        width(width),
        height(height),
        obstacle_type(obstacle_type),
        bomb_countdown(0),
        range(0),
        min_damage(0),
        max_damage(0),
        precision(0.0f) {}

/* Zona de Bomba */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
                     uint16_t height):
        type(type),
        position(position),
        id(0),
        player_type(PlayerType::UNKNOWN),
        weapon_model(WeaponModel::UNKNOWN),
        health(0),
        money(0),
        ammo(0),
        width(width),
        height(height),
        obstacle_type(ObstacleType::UNKNOWN),
        bomb_countdown(0),
        range(0),
        min_damage(0),
        max_damage(0),
        precision(0.0f) {}

/* Bala */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position):
        type(type),
        position(position),
        id(0),
        player_type(PlayerType::UNKNOWN),
        weapon_model(WeaponModel::UNKNOWN),
        health(0),
        money(0),
        ammo(0),
        width(0),
        height(0),
        obstacle_type(ObstacleType::UNKNOWN),
        bomb_countdown(0),
        range(0),
        min_damage(0),
        max_damage(0),
        precision(0.0f) {}

/* Bomba */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position,
                     uint16_t bomb_countdown):
        type(type),
        position(position),
        id(0),
        player_type(PlayerType::UNKNOWN),
        weapon_model(WeaponModel::UNKNOWN),
        health(0),
        money(0),
        ammo(0),
        width(0),
        height(0),
        obstacle_type(ObstacleType::UNKNOWN),
        bomb_countdown(bomb_countdown),
        range(0),
        min_damage(0),
        max_damage(0),
        precision(0.0f) {}

/* Arma */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position,
                     const WeaponDTO& weapon_dto):
        type(type),
        position(position),
        id(weapon_dto.id),
        player_type(PlayerType::UNKNOWN),
        weapon_model(weapon_dto.model),
        health(0),
        money(0),
        ammo(weapon_dto.ammo),
        width(0),
        height(0),
        obstacle_type(ObstacleType::UNKNOWN),
        bomb_countdown(0),
        range(weapon_dto.range),
        min_damage(weapon_dto.min_damage),
        max_damage(weapon_dto.max_damage),
        precision(weapon_dto.precision) {}
