#ifndef GAME_H
#define GAME_H

#include <utility>
#include <vector>

#include "action_DTO.h"

class Game {
private:
    std::vector<uint8_t> position;

public:
    /*
     * Constructor.
     **/
    Game(): position(2, 0) {}  // Vector con dos ceros: x = 0, y = 0

    /* Mover */
    bool move(Direction direction) {
        switch (direction) {
            case Direction::UP:
                position[1] += 1;
                break;
            case Direction::DOWN:
                position[1] -= 1;
                break;
            case Direction::LEFT:
                position[0] -= 1;
                break;
            case Direction::RIGHT:
                position[0] += 1;
                break;
            default:
                return false;
        }
        return true;
    }

    /* Getters */
    std::vector<uint8_t> get_position() const { return position; }
};

#endif  // GAME_H
