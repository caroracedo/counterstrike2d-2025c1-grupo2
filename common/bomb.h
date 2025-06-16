#ifndef BOMB_H
#define BOMB_H

#include <cmath>
#include <vector>

#include "constants.h"
#include "object.h"
#include "object_DTO.h"

#define BOMB_COUNTDOWN 30 * 15  // Si son 30FPS, serían 10 segundos

class Bomb: public Object {
private:
    int bomb_countdown = -1;

public:
    explicit Bomb(const std::vector<uint16_t>& position);

    ObjectDTO get_dto() const override;
    void start_countdown();
    bool update_countdown();
};

#endif  // BOMB_H
