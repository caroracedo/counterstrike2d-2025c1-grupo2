#ifndef BOMB_H
#define BOMB_H

#include <cmath>
#include <vector>

#include "constants.h"
#include "object.h"
#include "object_DTO.h"

class Bomb: public Object {
private:
    int bomb_countdown;

public:
    explicit Bomb(const std::vector<uint16_t>& position);

    ObjectDTO get_dto() const override;
    void start_countdown();
    bool update_countdown();
    bool is_active() const;
};

#endif  // BOMB_H
