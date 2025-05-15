#ifndef GAME_H
#define GAME_H

#include <set>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "obstacle.h"

class Game {
private:
    std::vector<uint8_t> position;
    std::set<Obstacle> obstacles;

public:
    /*
     * Constructor.
     **/
    Game(): position(2, 0), obstacles() {
        initialize_obstacles();
    }  // Vector con dos ceros: x = 0, y = 0

    /* Mover */
    bool move(Direction direction) {
        std::vector<uint8_t> new_position = position;
        switch (direction) {
            case Direction::UP:
                if (position[1] == 0) {
                    return false;  // No se puede mover hacia arriba
                }
                new_position[1] -= 1;
                break;
            case Direction::DOWN:
                if (position[1] == 255) {
                    return false;  // No se puede mover hacia abajo
                }
                new_position[1] += 1;
                break;
            case Direction::LEFT:
                if (position[0] == 0) {
                    return false;  // No se puede mover hacia la izquierda
                }
                new_position[0] -= 1;
                break;
            case Direction::RIGHT:
                if (position[0] == 255) {
                    return false;  // No se puede mover hacia la derecha
                }
                new_position[0] += 1;
                break;
            default:
                return false;
        }
        if (no_collisions(new_position)) {
            position = new_position;
            return true;
        }

        return false;
    }

    /* Getters */
    std::vector<uint8_t> get_position() const { return position; }

    bool no_collisions(std::vector<uint8_t> new_position) {
        // Aasumimos que el jugador tiene dimensiones 1x1 por el momento
        // Verificamos si la nueva posición colisiona con algún obstáculo
        return !std::any_of(obstacles.begin(), obstacles.end(), [&](const Obstacle& obstacle) {
            return new_position[0] >= obstacle.x && new_position[0] < obstacle.x + obstacle.width &&
                   new_position[1] >= obstacle.y && new_position[1] < obstacle.y + obstacle.height;
        });
    }

    void initialize_obstacles() {
        // Inicializamos algunos obstáculos de ejemplo
        obstacles.insert(Obstacle(5, 5, 1, 2));
        obstacles.insert(Obstacle(10, 10, 3, 3));
        obstacles.insert(Obstacle(15, 15, 4, 8));
    }
};

#endif  // GAME_H
