#ifndef GAME_H
#define GAME_H

#define MAX_POSITION 65535

#include <algorithm>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "obstacle.h"

class Game {  // Game sería (en este estado) un objeto (Jugador), deberíamos tener una clase Objeto
              // tipo más formal? Porque ObjectDTO serviría para serializar y deserializar
private:
    std::vector<uint16_t> position;
    std::vector<Obstacle> obstacles;

public:
    /*
     * Constructor.
     **/
    Game(): position(2, 0), obstacles() { initialize_obstacles(); }

    /* Mover */
    bool move(Direction direction) {
        std::vector<uint16_t> new_position = position;
        switch (direction) {
            case Direction::UP:
                if (position[1] == 0) {
                    return false;  // No se puede mover hacia arriba
                }
                new_position[1] -= 1;
                break;
            case Direction::DOWN:
                if (position[1] == MAX_POSITION) {
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
                if (position[0] == MAX_POSITION) {
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
    std::vector<uint16_t> get_position() const { return position; }

    bool no_collisions(std::vector<uint16_t> new_position) {
        // Asumimos que el jugador tiene dimensiones 1x1 por el momento
        // Verificamos si la nueva posición colisiona con algún obstáculo
        return !std::any_of(obstacles.begin(), obstacles.end(), [&](const Obstacle& obstacle) {
            return new_position[0] >= obstacle.x && new_position[0] < obstacle.x + obstacle.width &&
                   new_position[1] >= obstacle.y && new_position[1] < obstacle.y + obstacle.height;
        });
    }

    void initialize_obstacles() {
        // Inicializamos algunos obstáculos de ejemplo
        obstacles.push_back(Obstacle(1, 1));
        obstacles.push_back(Obstacle(5, 5, 1, 2));
        obstacles.push_back(Obstacle(10, 10, 3, 3));
        obstacles.push_back(Obstacle(15, 15, 4, 8));
    }
};

#endif  // GAME_H
