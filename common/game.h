#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 100  // posición máxima en la matriz
#define MOVE_STEP 1      // paso de movimiento
#define CELL_SIZE 1      // tamaño de cada celda en la matriz
#define PLAYER_SIZE 2    // tamaño del jugador
#define BULLET_SIZE 1    // tamaño de la bala

#include <algorithm>
#include <cstdint>
#include <set>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "object_DTO.h"
#include "obstacle.h"

class Game {
private:
    std::vector<std::vector<std::vector<ObjectDTO>>> matrix;
    std::vector<ObjectDTO> objects;

    std::pair<uint16_t, uint16_t> get_cell_from_position(const std::vector<uint16_t>& position) {
        return {position[0] / CELL_SIZE, position[1] / CELL_SIZE};
    }

    std::set<ObjectDTO> get_adyacent_objects(const std::pair<uint16_t, uint16_t>& cell) {
        /*
            TODO: mucho código repetido, ver si se puede refactorizar
        */

        std::set<ObjectDTO> objects_set;
        // agrego al set los objetos de la celda actual
        auto& const cell_objects = matrix[cell.first][cell.second];
        for (const auto& obj: cell_objects) {
            objects_set.insert(obj);
        }
        // Agrego a objects_set los objetos en la celdas adyacentes
        // celda de arriba
        if (cell.first > 0) {
            auto& cell_objects_up = matrix[cell.first - 1][cell.second];
            objects_set.insert(cell_objects_up.begin(), cell_objects_up.end());
        }
        // celda de abajo
        if (cell.first < MATRIX_SIZE - 1) {
            auto& cell_objects_down = matrix[cell.first + 1][cell.second];
            objects_set.insert(cell_objects_down.begin(), cell_objects_down.end());
        }
        // celda de izquierda
        if (cell.second > 0) {
            auto& cell_objects_left = matrix[cell.first][cell.second - 1];
            objects_set.insert(cell_objects_left.begin(), cell_objects_left.end());
        }
        // celda de derecha
        if (cell.second < MATRIX_SIZE - 1) {
            auto& cell_objects_right = matrix[cell.first][cell.second + 1];
            objects_set.insert(cell_objects_right.begin(), cell_objects_right.end());
        }
        // celda diagonal superior izquierda
        if (cell.first > 0 && cell.second > 0) {
            auto& cell_objects_diag_up_left = matrix[cell.first - 1][cell.second - 1];
            objects_set.insert(cell_objects_diag_up_left.begin(), cell_objects_diag_up_left.end());
        }
        // celda diagonal superior derecha
        if (cell.first > 0 && cell.second < MATRIX_SIZE - 1) {
            auto& cell_objects_diag_up_right = matrix[cell.first - 1][cell.second + 1];
            objects_set.insert(cell_objects_diag_up_right.begin(),
                               cell_objects_diag_up_right.end());
        }
        // celda diagonal inferior izquierda
        if (cell.first < MATRIX_SIZE - 1 && cell.second > 0) {
            auto& cell_objects_diag_down_left = matrix[cell.first + 1][cell.second - 1];
            objects_set.insert(cell_objects_diag_down_left.begin(),
                               cell_objects_diag_down_left.end());
        }
        // celda diagonal inferior derecha
        if (cell.first < MATRIX_SIZE - 1 && cell.second < MATRIX_SIZE - 1) {
            auto& cell_objects_diag_down_right = matrix[cell.first + 1][cell.second + 1];
            objects_set.insert(cell_objects_diag_down_right.begin(),
                               cell_objects_diag_down_right.end());
        }

        return objects_set;
    }

    std::pair<bool, std::vector<uint16_t>> _move(Direction direction,
                                                 std::vector<uint16_t> position, uint16_t size,
                                                 uint16_t delta) {
        std::vector<uint16_t> new_position = position;
        uint16_t MAX_POSITION = MATRIX_SIZE * CELL_SIZE - size;
        switch (direction) {
            case Direction::UP:
                if (position[1] == 0) {
                    return {false, position};  // No se puede mover hacia arriba
                }
                uint32_t temp = (position[1] > delta) ? position[1] - delta : 0;
                new_position[1] = static_cast<uint16_t>(temp);
                break;
            case Direction::DOWN:
                if (position[1] == MAX_POSITION) {
                    return {false, position};  // No se puede mover hacia abajo
                }
                uint32_t temp = static_cast<uint32_t>(position[1]) + delta;
                new_position[1] =
                        (temp > MAX_POSITION) ? MAX_POSITION : static_cast<uint16_t>(temp);
                break;
            case Direction::LEFT:
                if (position[0] == 0) {
                    return {false, position};  // No se puede mover hacia la izquierda
                }
                uint32_t temp = (position[0] > delta) ? position[0] - delta : 0;
                new_position[0] = static_cast<uint16_t>(temp);
                break;
            case Direction::RIGHT:
                if (position[0] == MAX_POSITION) {
                    return {false, position};  // No se puede mover hacia la derecha
                }
                uint32_t temp = static_cast<uint32_t>(position[0]) + delta;
                new_position[0] =
                        (temp > MAX_POSITION) ? MAX_POSITION : static_cast<uint16_t>(temp);
                break;
            default:
                return {false, position};
        }

        // Obtenemos la posición máxima que se puede mover sin colisionar
        std::vector<uint16_t> max_position = get_max_position(position, new_position, size);

        return {max_position == position, max_position};
    }

    std::vector<uint16_t> get_max_position(const std::vector<uint16_t>& old_position,
                                           const std::vector<uint16_t>& new_position,
                                           const uint16_t& size) {
        std::vector<uint16_t> max_position = old_position;
        auto cell = get_cell_from_position(new_position);
        std::set<ObjectDTO> ady_objects = get_adyacent_objects(cell);

        // Por cada posición entre la posición vieja y la nueva, verifico si hay objetos que
        // colisionen

        // Movimiento hacia arriba
        for (uint16_t j = old_position[1]; j >= new_position[1]; --j) {
            std::vector<uint16_t> test_position = {old_position[0], j};
            if (collides(test_position, ady_objects, size)) {
                break;
            } else {
                max_position = test_position;
            }
        }

        // Movimiento hacia abajo
        for (uint16_t j = old_position[1]; j <= new_position[1]; ++j) {
            std::vector<uint16_t> test_position = {old_position[0], j};
            if (collides(test_position, ady_objects, size)) {
                break;
            } else {
                max_position = test_position;
            }
        }

        // Movimiento hacia la izquierda
        for (uint16_t i = old_position[0]; i >= new_position[0]; --i) {
            std::vector<uint16_t> test_position = {i, max_position[1]};
            if (collides(test_position, ady_objects, size)) {
                break;
            } else {
                max_position = test_position;
            }
        }

        // Movimiento hacia la derecha
        for (uint16_t i = old_position[0]; i <= new_position[0]; ++i) {
            std::vector<uint16_t> test_position = {i, max_position[1]};
            if (collides(test_position, ady_objects, size)) {
                break;
            } else {
                max_position = test_position;
            }
        }

        return max_position;
    }

    bool collides(std::vector<uint16_t> new_position, const std::set<ObjectDTO>& objects,
                  const uint16_t& size) {
        // Devuelve true si algún objeto colisiona con la nueva posición
        return std::any_of(objects.begin(), objects.end(), [&](const ObjectDTO& obj) {
            return new_position[0] < obj.position[0] + obj.width &&
                   new_position[0] + size > obj.position[0] &&
                   new_position[1] < obj.position[1] + obj.height &&
                   new_position[1] + size > obj.position[1];
        });
    }


public:
    /*
     * Constructor.
     **/

    Game():
            matrix(MATRIX_SIZE,
                   std::vector<std::vector<ObjectDTO>>(
                           MATRIX_SIZE, std::vector<ObjectDTO>(MATRIX_SIZE, ObjectDTO()))) {}

    bool move(Direction direction, const uint16_t& id) {
        // TODO: verificar que el id corresponda a un jugador y no a una bala u obstáculo

        auto it = std::find_if(objects.begin(), objects.end(),
                               [id](const ObjectDTO& obj) { return obj.id == id; });
        if (it != objects.end()) {
            std::vector<uint16_t> old_position = it->position;
            auto move_result = _move(direction, it->position, PLAYER_SIZE, MOVE_STEP);
            if (move_result.first) {
                // Quitar el objeto de la celda anterior
                auto& old_cell = matrix[old_position[0]][old_position[1]];
                old_cell.erase(std::remove_if(old_cell.begin(), old_cell.end(),
                                              [id](const ObjectDTO& obj) { return obj.id == id; }),
                               old_cell.end());

                // Actualizar la posición del objeto
                it->position = move_result.second;

                // Calcular la nueva celda
                auto new_cell = get_cell_from_position(it->position);

                // Agregar el objeto a la nueva celda
                matrix[new_cell.first][new_cell.second].push_back(*it);

                return true;
            }
        }
        return false;
    }

    std::vector<ObjectDTO>& get_objects() { return objects; }
};

#endif  // GAME_H
