#ifndef GAME_H
#define GAME_H

#include "action_DTO.h"

class Game {
private:
    int x, y; // Posición

public:
    /*
     * Constructor.
     **/
    Game(): x(0), y(0) {}; // Inicia en origen de coordenadas

    /* Mover */
    bool move(Direction direction) {
        switch (direction) {
            case UP:
                y += 1;
                break;
            case DOWN:
                y -= 1;
                break;
            case LEFT:
                x -= 1;
                break;
            case RIGHT:
                x += 1;
                break;
        }
    }
};

#endif  // GAME_H