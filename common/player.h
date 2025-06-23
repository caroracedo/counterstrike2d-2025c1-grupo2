#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "constants.h"
#include "object.h"
#include "weapon.h"
#include "weapon_DTO.h"
#include "weapon_shop.h"

class Player: public Object {
private:
    PlayerType player_type;
    PlayerSkin player_skin;
    uint16_t health;
    bool infinite_health = false;
    uint16_t money;
    bool infinite_money = false;
    uint16_t radius = PLAYER_RADIUS;
    float angle = INITIAL_PLAYER_ANGLE;

    WeaponShop& weapon_shop;

    Weapon knife;
    Weapon primary_weapon;
    Weapon secondary_weapon;
    bool has_bomb = false;

    Weapon* current;

    WeaponDTO drop_primary_weapon();

public:
    Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type,
           PlayerSkin player_skin, uint8_t health, uint16_t initial_money, WeaponShop& weapon_shop);

    ObjectDTO get_dto() const override;

    bool is_alive() const;

    WeaponDTO get_current_weapon() const;
    void change_weapon();
    std::string get_current_weapon_name() const;
    std::pair<WeaponDTO, bool> drop_weapons();
    WeaponDTO pick_up_weapon(const WeaponDTO& weapon_dto);

    WeaponDTO buy_weapon(const WeaponModel& weapon_model);
    bool buy_ammo(WeaponType weapon, uint16_t ammo_amount);
    void add_money(uint16_t amount);

    bool shoot();

    bool can_plant_bomb() const;
    void plant_bomb();
    void set_bomb();

    PlayerSkin get_player_skin() const;
    PlayerType get_player_type() const;
    void switch_player_type();
    void switch_player_skin();

    void take_damage(uint16_t damage);
    void cure(uint16_t health_amount);

    std::vector<uint16_t> get_next_position(Direction direction) const;
    void rotate(float new_angle);
    float get_angle() const { return angle; }

    // Cheats
    void do_health_cheat() { infinite_health = true; }
    void do_ammo_cheat() {
        primary_weapon.set_infinite_ammo();
        secondary_weapon.set_infinite_ammo();
    }
    void do_money_cheat() { infinite_money = true; }
};

#endif  // PLAYER
