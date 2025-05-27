#ifndef PLAYER_H
#define PLAYER_H

#include <cstdint>
#include <vector>

#include "object.h"

#define PLAYER_SIZE 32  // tamaño del jugador

class Player: public Object {
private:
    uint16_t health;

public:
    Player(uint16_t id, const std::vector<uint16_t>& position):
            Object(ObjectType::PLAYER, id, PLAYER_SIZE, PLAYER_SIZE, position), health(100) {}

    void take_damage(uint16_t damage) override {
        if (damage >= health) {
            health = 0;  // El jugador muere
        } else {
            health -= damage;  // Resta el daño a la salud del jugador
        }

        std::cout << "\tJugador " << id << " recibió daño. Salud restante: " << health << std::endl;
    }

    bool is_alive() const override {
        return health > 0;  // El jugador está vivo si su salud es mayor a 0
    }
};

#endif  // PLAYER
