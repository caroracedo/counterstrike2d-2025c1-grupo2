#ifndef GAME_H
#define GAME_H

#include <utility>

#include "action_DTO.h"

class Game {
private:
    int x, y;  // Posición

public:
    /*
     * Constructor.
     **/
    Game(): x(0), y(0) {}  // Inicia en origen de coordenadas

    /* Mover */
    bool move(Direction direction) {
        switch (direction) {
            case Direction::UP:
                y += 1;
                break;
            case Direction::DOWN:
                y -= 1;
                break;
            case Direction::LEFT:
                x -= 1;
                break;
            case Direction::RIGHT:
                x += 1;
                break;
        }
        // TODO: cambiar -> lo agregue para que cppcheck no tire error
        return false;
    }
};

#endif  // GAME_H
