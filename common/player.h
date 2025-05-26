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
};

#endif  // PLAYER
