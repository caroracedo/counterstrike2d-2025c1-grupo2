#ifndef BOMB_H
#define BOMB_H

#include <cmath>
#include <vector>

#include "object.h"
#include "object_DTO.h"

#define BOMB_RADIUS 16
#define BOMB_COUNTDOWN 30 * 10  // Si son 30FPS, serían 10 segundos

class Bomb: public Object {
private:
    int bomb_countdown = -1;

public:
    explicit Bomb(const std::vector<uint16_t>& position):
            Object(ObjectType::BOMB, 0, position, BOMB_RADIUS, BOMB_RADIUS) {}

    ObjectDTO get_dto() const override {
        return ObjectDTO(object_type, position, std::floor(bomb_countdown / 30));
    }

    void start_countdown() { bomb_countdown = BOMB_COUNTDOWN; }
    bool update_countdown() { return bomb_countdown >= 0 && --bomb_countdown == 0; }
};

#endif  // BOMB_H
