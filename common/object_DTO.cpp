#include "object_DTO.h"
/* Unknown */

ObjectDTO::ObjectDTO():
        type(ObjectType::UNKNOWN),
        position(),
        id(),
        player_type(),
        player_skin(),
        health(),
        money(),
        angle(),
        weapon_model(),
        ammo(),
        width(),
        height(),
        obstacle_type(),
        bomb_countdown() {}

/* Player */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t id,
                     PlayerType player_type, PlayerSkin player_skin, WeaponModel weapon_model,
                     uint16_t health, uint16_t money, uint16_t ammo, float angle):
        ObjectDTO() {
    this->type = type;
    this->position = position;
    this->id = id;
    this->player_type = player_type;
    this->player_skin = player_skin;
    this->angle = angle;
    this->weapon_model = weapon_model;
    this->health = health;
    this->money = money;
    this->ammo = ammo;
}

/* Obstáculo */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
                     uint16_t height, ObstacleType obstacle_type):
        ObjectDTO() {
    this->type = type;
    this->position = position;
    this->width = width;
    this->height = height;
    this->obstacle_type = obstacle_type;
}

/* Zona de Bomba */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position, uint16_t width,
                     uint16_t height):
        ObjectDTO() {
    this->type = type;
    this->position = position;
    this->width = width;
    this->height = height;
}

/* Bala */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position): ObjectDTO() {
    this->type = type;
    this->position = position;
}

/* Bomba */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position,
                     uint16_t bomb_countdown):
        ObjectDTO() {
    this->type = type;
    this->position = position;
    this->bomb_countdown = bomb_countdown;
}

/* Arma */
ObjectDTO::ObjectDTO(ObjectType type, const std::vector<uint16_t>& position,
                     const WeaponModel& weapon_model):
        ObjectDTO() {
    this->type = type;
    this->position = position;
    this->weapon_model = weapon_model;
}
