#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <vector>

#include "object.h"
#include "weapon.h"

#define PLAYER_SIZE 32  // tamaño del jugador

class Player: public Object {
private:
    enum class WeaponType : uint8_t { PRIMARY, SECONDARY, KNIFE, UNKNOWN };
    enum class PlayerType : uint8_t { TERRORIST, COUNTERTERRORIST, UNKNOWN };

    PlayerType type;
    uint16_t health;
    Weapon primary_weapon;  // El jugador tiene un arma
    Weapon secondary_weapon;
    Weapon knife;

    uint16_t money;


    Weapon current;

public:
    // Player(uint16_t id, const std::vector<uint16_t>& position, PlayerType type):
    //         Object(ObjectType::PLAYER, id, PLAYER_SIZE, PLAYER_SIZE, position), type(type),
    //         health(100), primary_weapon(), secondary_weapon({GLOCK}), knife({KNIFE}) {}

    void take_damage(uint16_t damage) override {
        if (health > damage) {
            health -= damage;
        } else {
            health = 0;  // El jugador muere
        }
    }
    bool is_alive() const override { return health > 0; }

    void select_weapon(WeaponType weapon_type) {
        current = (weapon_type == WeaponType::PRIMARY)   ? primary_weapon :
                  (weapon_type == WeaponType::SECONDARY) ? secondary_weapon :
                                                           knife;
    }

    void buy_weapon(const Weapon& weapon) {}
};

#endif  // PLAYER
