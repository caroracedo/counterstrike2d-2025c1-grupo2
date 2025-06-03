#ifndef BOMB_H
#define BOMB_H

#include <vector>

#include "object.h"
#include "object_DTO.h"

#define BOMB_RADIUS 16
// #define BOMB_COUNTDOWN 300// Si son 30FPS, serían 10 segundos
#define BOMB_COUNTDOWN 5

class Bomb: public Object {
private:
    int bomb_countdown = -1;

public:
    explicit Bomb(const std::vector<uint16_t>& position):
            Object(ObjectType::BOMB, 0, position, BOMB_RADIUS, BOMB_RADIUS) {}

    ObjectDTO get_dto() const override { return ObjectDTO(object_type, position, bomb_countdown); }

    void start_countdown() { bomb_countdown = BOMB_COUNTDOWN; }
    bool update_countdown() { return bomb_countdown >= 0 && --bomb_countdown == 0; }
};

#endif  // BOMB_H
