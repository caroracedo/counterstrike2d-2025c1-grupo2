#include "player.h"

#include <iostream>

Player::Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type, uint8_t health,
               uint16_t initial_money, WeaponShop& weapon_shop):
        Object(ObjectType::PLAYER, id, position, PLAYER_RADIUS * 2, PLAYER_RADIUS * 2),
        player_type(type),
        health(health),
        money(initial_money),
        weapon_shop(weapon_shop),
        knife(weapon_shop.give_weapon(WeaponModel::KNIFE)),
        secondary_weapon(weapon_shop.give_weapon(WeaponModel::GLOCK)),
        current(&secondary_weapon) {}

ObjectDTO Player::get_dto() const {
    return ObjectDTO(object_type, position, id, player_type, current->get_model(), health, money,
                     current->get_ammo(), angle);
}

bool Player::is_alive() const { return health > 0; }

WeaponDTO Player::get_current_weapon() const { return current->get_weapon_dto(); }

void Player::change_weapon() {
    // primary_weapon -> secondary_weapon -> knife -> primary_weapon
    if (current == &primary_weapon) {
        current = &secondary_weapon;
    } else if (current == &secondary_weapon) {
        current = &knife;
    } else if (current == &knife && primary_weapon.get_model() != WeaponModel::UNKNOWN) {
        current = &primary_weapon;
    } else {
        current = &secondary_weapon;
    }
}

std::string Player::get_current_weapon_name() const {
    switch (current->get_model()) {
        case WeaponModel::KNIFE:
            return "Knife";
        case WeaponModel::GLOCK:
            return "Glock";
        case WeaponModel::AK47:
            return "AK-47";
        case WeaponModel::M3:
            return "M3";
        case WeaponModel::AWP:
            return "AWP";
        case WeaponModel::BOMB:
            return "Bomb";
        default:
            return "Unknown Weapon";
    }
}

std::pair<WeaponDTO, bool> Player::drop_weapons() {
    WeaponDTO weapon = drop_primary_weapon();
    if (has_bomb) {
        has_bomb = false;
        std::cout << "[PLAYER] Bomb dropped." << std::endl;
        return {weapon, true};
    }
    current = &secondary_weapon;
    return {weapon, false};
}

WeaponDTO Player::pick_up_weapon(const WeaponDTO& weapon_dto) {
    WeaponDTO weapon_dropped;
    if (weapon_dto.model == WeaponModel::BOMB) {
        has_bomb = (player_type == PlayerType::TERRORIST) ? true : false;
        return weapon_dropped;
    }
    if (weapon_dto.model == WeaponModel::UNKNOWN) {
        return weapon_dropped;
    }
    weapon_dropped = drop_primary_weapon();
    primary_weapon = Weapon(weapon_dto);
    return weapon_dropped;
}

bool Player::buy_weapon(const WeaponModel& weapon_model) {
    std::pair<uint16_t, Weapon> purchase = weapon_shop.sell_weapon(weapon_model, money);
    if (purchase.second.get_model() == WeaponModel::UNKNOWN) {
        return false;
    }
    money -= purchase.first;
    primary_weapon = purchase.second;
    return true;
}

bool Player::buy_ammo(WeaponType weapon_type, uint16_t ammo_amount) {
    std::pair<uint16_t, bool> purchase = weapon_shop.sell_ammo(ammo_amount, money);
    if (!purchase.second) {
        return false;
    }
    money -= purchase.first;
    switch (weapon_type) {
        case WeaponType::PRIMARY:
            primary_weapon.add_ammo(ammo_amount);
            break;
        case WeaponType::SECONDARY:
            secondary_weapon.add_ammo(ammo_amount);
            break;
        default:
            return false;  // Tipo de arma desconocido
    }
    return true;
}

void Player::add_money(uint16_t amount) { money += amount; }

bool Player::shoot() { return current->shoot(); }

bool Player::can_plant_bomb() const { return has_bomb; }

void Player::plant_bomb() { has_bomb = false; }

void Player::set_bomb() { has_bomb = true; }

PlayerType Player::get_player_type() const { return player_type; }

void Player::switch_player_type() {
    player_type = (player_type == PlayerType::TERRORIST)        ? PlayerType::COUNTERTERRORIST :
                  (player_type == PlayerType::COUNTERTERRORIST) ? PlayerType::TERRORIST :
                                                                  PlayerType::UNKNOWN;
}

void Player::take_damage(uint16_t damage) {
    if (health > damage) {
        health -= damage;
    } else {
        health = 0;
    }
}

void Player::cure(uint16_t health_amount) { health = health_amount; }

std::vector<uint16_t> Player::get_next_position(Direction direction) const {
    uint16_t min_position = radius;
    uint16_t max_position = MATRIX_SIZE * CELL_SIZE - radius;
    std::vector<uint16_t> new_position = position;
    switch (direction) {
        case Direction::UP:
            new_position[1] = (position[1] > min_position + MOVE_STEP) ? position[1] - MOVE_STEP :
                                                                         min_position;
            break;
        case Direction::DOWN:
            new_position[1] = (position[1] + MOVE_STEP < max_position) ? position[1] + MOVE_STEP :
                                                                         max_position;
            break;
        case Direction::LEFT:
            new_position[0] = (position[0] > min_position + MOVE_STEP) ? position[0] - MOVE_STEP :
                                                                         min_position;
            break;
        case Direction::RIGHT:
            new_position[0] = (position[0] + MOVE_STEP < max_position) ? position[0] + MOVE_STEP :
                                                                         max_position;
            break;
        default:
            break;
    }
    return new_position;
}

void Player::rotate(float new_angle) { angle = new_angle; }

WeaponDTO Player::drop_primary_weapon() {
    WeaponDTO weapon = primary_weapon.get_weapon_dto();
    primary_weapon = Weapon();
    return weapon;
}
