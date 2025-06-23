#include "player.h"

#include <iostream>

Player::Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type,
               PlayerSkin player_skin, uint8_t health, uint16_t initial_money,
               WeaponShop& weapon_shop):
        Object(ObjectType::PLAYER, id, position, PLAYER_WIDTH, PLAYER_HEIGHT),
        player_type(type),
        player_skin(player_skin),
        health(health),
        infinite_health(false),
        money(initial_money),
        infinite_money(false),
        radius(PLAYER_RADIUS),
        angle(INITIAL_PLAYER_ANGLE),
        weapon_shop(weapon_shop),
        knife(weapon_shop.give_weapon(WeaponModel::KNIFE)),
        secondary_weapon(weapon_shop.give_weapon(WeaponModel::GLOCK)),
        has_bomb(false),
        current(&secondary_weapon) {}

ObjectDTO Player::get_dto() const {
    return ObjectDTO(object_type, position, id, player_type, player_skin, current->get_model(),
                     health, money, current->get_ammo(), angle);
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

std::pair<WeaponDTO, bool> Player::drop_weapons() {
    WeaponDTO weapon = drop_primary_weapon();
    if (has_bomb) {
        has_bomb = false;
        current = &secondary_weapon;
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

WeaponDTO Player::buy_weapon(const WeaponModel& weapon_model) {
    /*
        Compra el arma especificada y devuelve un DTO con el arma que ya tenía (si es que tenía)
    */
    WeaponDTO old_weapon;
    std::pair<uint16_t, Weapon> purchase = weapon_shop.sell_weapon(weapon_model, money);
    if (purchase.second.get_model() == WeaponModel::UNKNOWN) {
        return old_weapon;
    }
    old_weapon = drop_primary_weapon();
    if (!infinite_money) {
        money -= purchase.first;
    }
    primary_weapon = purchase.second;
    return old_weapon;
}

bool Player::buy_ammo(WeaponType weapon_type, uint16_t ammo_amount) {
    std::pair<uint16_t, bool> purchase = weapon_shop.sell_ammo(ammo_amount, money);
    if (!purchase.second) {
        return false;
    }
    if (!infinite_money) {
        money -= purchase.first;
    }
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

PlayerSkin Player::get_player_skin() const { return player_skin; }

PlayerType Player::get_player_type() const { return player_type; }

void Player::switch_player_type() {
    player_type = (player_type == PlayerType::TERRORIST)        ? PlayerType::COUNTERTERRORIST :
                  (player_type == PlayerType::COUNTERTERRORIST) ? PlayerType::TERRORIST :
                                                                  PlayerType::UNKNOWN;
}

void Player::switch_player_skin() {
    std::unordered_map<PlayerSkin, PlayerSkin> opposite_skin = {
            {PlayerSkin::PHEONIX, PlayerSkin::SEALFORCE},
            {PlayerSkin::SEALFORCE, PlayerSkin::PHEONIX},
            {PlayerSkin::L337KREW, PlayerSkin::GERMANGSG9},
            {PlayerSkin::GERMANGSG9, PlayerSkin::L337KREW},
            {PlayerSkin::ARTICAVENGER, PlayerSkin::UKSAS},
            {PlayerSkin::UKSAS, PlayerSkin::ARTICAVENGER},
            {PlayerSkin::GUERRILLA, PlayerSkin::FRENCHGIGN},
            {PlayerSkin::FRENCHGIGN, PlayerSkin::GUERRILLA},
            {PlayerSkin::UNKNOWN, PlayerSkin::UNKNOWN}};
    PlayerSkin new_skin = opposite_skin[player_skin];
    player_skin = new_skin;
}

void Player::take_damage(uint16_t damage) {
    if (infinite_health) {
        return;
    }
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

float Player::get_angle() const { return angle; }

void Player::do_health_cheat() {
    infinite_health = true;
    health = 9999;
    std::cout << "New health: " << health << std::endl;
}

void Player::do_ammo_cheat() {
    primary_weapon.set_infinite_ammo();
    secondary_weapon.set_infinite_ammo();
}

void Player::do_money_cheat() {
    infinite_money = true;
    money = 9999;
}

WeaponDTO Player::drop_primary_weapon() {
    WeaponDTO weapon = primary_weapon.get_weapon_dto();
    primary_weapon = Weapon();
    return weapon;
}
