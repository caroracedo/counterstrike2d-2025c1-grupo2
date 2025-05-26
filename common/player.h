#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include <cstdint>

#define PLAYER_SIZE 32  // tamaño del jugador

class Player : public Object {
private:
    uint16_t health;

public:
    Player(uint16_t id, const std::vector<uint16_t>& position)
        : Object(ObjectType::PLAYER, id, PLAYER_SIZE, PLAYER_SIZE, position), health(100) {}


    void move(const std::vector<uint16_t>& new_position, const bool was_hit) {
        // Implementación del movimiento del jugador
        // Actualiza la posición del jugador
        // ...
    }

};

#endif  // PLAYER