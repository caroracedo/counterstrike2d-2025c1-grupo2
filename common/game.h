#ifndef GAME_H
#define GAME_H

#define MAX_POSITION 65535
#define PLAYER_SIZE 32
#define BULLET_SIZE 8
#define MOVE_DELTA 5


#include <algorithm>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "obstacle.h"

class Game {
private:
    std::vector<uint16_t> position;
    std::vector<Obstacle> obstacles;

    bool _move(Direction direction, uint16_t size, uint16_t delta) {
        std::vector<uint16_t> new_position = position;
        uint16_t MAX_POS = MAX_POSITION - size;  // Máxima posición permitida

        switch (direction) {
            case Direction::UP:
                if (position[1] == 0) {
                    return false;  // No se puede mover hacia arriba
                }
                {
                    uint32_t temp = (position[1] > delta) ? position[1] - delta : 0;
                    new_position[1] = static_cast<uint16_t>(temp);
                }
                break;
            case Direction::DOWN:
                if (position[1] == MAX_POS) {
                    return false;  // No se puede mover hacia abajo
                }
                {
                    uint32_t temp = static_cast<uint32_t>(position[1]) + delta;
                    new_position[1] = (temp > MAX_POS) ? MAX_POS : static_cast<uint16_t>(temp);
                }
                break;
            case Direction::LEFT:
                if (position[0] == 0) {
                    return false;  // No se puede mover hacia la izquierda
                }
                {
                    uint32_t temp = (position[0] > delta) ? position[0] - delta : 0;
                    new_position[0] = static_cast<uint16_t>(temp);
                }
                break;
            case Direction::RIGHT:
                if (position[0] == MAX_POS) {
                    return false;  // No se puede mover hacia la derecha
                }
                {
                    uint32_t temp = static_cast<uint32_t>(position[0]) + delta;
                    new_position[0] = (temp > MAX_POS) ? MAX_POS : static_cast<uint16_t>(temp);
                }
                break;
            default:
                return false;
        }

        std::vector<uint16_t> max_position = get_max_position(position, new_position, size);
        if (max_position == position) {
            return false;  // No se puede mover
        }
        position = max_position;
        return true;
    }


    bool no_collisions(std::vector<uint16_t> new_position, uint16_t size) {
        // Calcula las cuatro esquinas del objeto
        std::vector<std::pair<uint16_t, uint16_t>> corners = {
                {new_position[0], new_position[1]},
                {static_cast<uint16_t>(new_position[0] + size - 1), new_position[1]},
                {new_position[0], static_cast<uint16_t>(new_position[1] + size - 1)},
                {static_cast<uint16_t>(new_position[0] + size - 1),
                 static_cast<uint16_t>(new_position[1] + size - 1)}};

        // Verifica que ninguna esquina colisione con un obstáculo
        return !std::any_of(obstacles.begin(), obstacles.end(), [&](const Obstacle& obstacle) {
            return std::any_of(corners.begin(), corners.end(),
                               [&](const std::pair<uint16_t, uint16_t>& corner) {
                                   return corner.first >= obstacle.x &&
                                          corner.first < obstacle.x + obstacle.width &&
                                          corner.second >= obstacle.y &&
                                          corner.second < obstacle.y + obstacle.height;
                               });
        });
    }

    std::vector<uint16_t> get_max_position(std::vector<uint16_t> old_position,
                                           std::vector<uint16_t> new_position, uint16_t size) {
        // Devuelve hasta donde puede avanzar sin colisionar
        std::vector<uint16_t> max_position = old_position;

        // Por cada posicion entre old_position y new_position, verificamos si hay colisión
        // Por el momento, solo hay movimiento vertical y horizontal

        // Hacia arriba
        for (uint16_t j = old_position[1]; j >= new_position[1]; --j) {
            std::vector<uint16_t> test_position = {old_position[0], j};
            if (no_collisions(test_position, size)) {
                max_position = test_position;
            } else {
                break;  // Salimos del bucle si hay colisión
            }
        }

        // Hacia abajo
        for (uint16_t j = old_position[1]; j <= new_position[1]; ++j) {
            std::vector<uint16_t> test_position = {old_position[0], j};
            if (no_collisions(test_position, size)) {
                max_position = test_position;
            } else {
                break;  // Salimos del bucle si hay colisión
            }
        }

        // Hacia la izquierda
        for (uint16_t i = old_position[0]; i >= new_position[0]; --i) {
            std::vector<uint16_t> test_position = {i, old_position[1]};
            if (no_collisions(test_position, size)) {
                max_position = test_position;
            } else {
                break;  // Salimos del bucle si hay colisión
            }
        }

        // Hacia la derecha
        for (uint16_t i = old_position[0]; i <= new_position[0]; ++i) {
            std::vector<uint16_t> test_position = {i, old_position[1]};
            if (no_collisions(test_position, size)) {
                max_position = test_position;
            } else {
                break;  // Salimos del bucle si hay colisión
            }
        }

        return max_position;  // Devolvemos la posición máxima alcanzada sin colisión
    }

    void initialize_obstacles() {
        // Inicializamos algunos obstáculos de ejemplo
        obstacles.push_back(Obstacle(50, 50, 100));
        obstacles.push_back(Obstacle(200, 200, 50, 70));
        obstacles.push_back(Obstacle(10, 10, 10));
    }


public:
    /*
     * Constructor.
     **/
    Game(): position(2, 0), obstacles() { initialize_obstacles(); }

    /* Mover */
    bool move(Direction direction) { return _move(direction, PLAYER_SIZE, MOVE_DELTA); }

    bool shoot(Direction direction, WeaponDTO weapon) {
        return _move(direction, BULLET_SIZE, weapon.range);
    }

    /* Getters */
    std::vector<uint16_t> get_position() const { return position; }
};

#endif  // GAME_H
