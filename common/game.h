#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 200  // posición máxima en la matriz
#define MOVE_STEP 5      // paso de movimiento
#define CELL_SIZE 56     // tamaño de cada celda en la matriz
#define PLAYER_SIZE 32   // tamaño del jugador
#define BULLET_SIZE 8    // tamaño de la bala

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "object.h"
#include "object_DTO.h"
#include "obstacle.h"
#include "player.h"
#include "weapon.h"

class Game {
private:
    std::vector<std::vector<std::vector<Object>>> matrix;
    std::vector<Object> objects;
    enum class CollisionType { NONE, BULLET, OTHER };
    int bullet_id = 0;

    std::pair<uint16_t, uint16_t> get_cell_from_position(const std::vector<uint16_t>& position) {
        uint16_t x = position[0] / CELL_SIZE;
        uint16_t y = position[1] / CELL_SIZE;
        if (x >= MATRIX_SIZE)
            x = MATRIX_SIZE - 1;
        if (y >= MATRIX_SIZE)
            y = MATRIX_SIZE - 1;
        return {x, y};
    }

    std::vector<std::pair<uint16_t, uint16_t>> get_cells_within_radius(
            const std::pair<uint16_t, uint16_t>& cell, uint16_t radius = 2) {
        /*
         *    Obtiene las celdas adyacentes a la celda dada dentro de un radio especificado.
         */

        std::vector<std::pair<uint16_t, uint16_t>> cells;
        for (int dx = -static_cast<int>(radius); dx <= static_cast<int>(radius); ++dx) {
            for (int dy = -static_cast<int>(radius); dy <= static_cast<int>(radius); ++dy) {
                int nx = static_cast<int>(cell.first) + dx;
                int ny = static_cast<int>(cell.second) + dy;
                if (nx >= 0 && nx < MATRIX_SIZE && ny >= 0 && ny < MATRIX_SIZE) {
                    cells.emplace_back(static_cast<uint16_t>(nx), static_cast<uint16_t>(ny));
                }
            }
        }
        return cells;
    }

    std::set<Object> get_adyacent_objects(const std::pair<uint16_t, uint16_t>& cell) {
        /*
         *    Obtiene los objetos en las celdas adyacentes a la celda dada
         */

        // Vector con todas las celdas adyacentes y las adyacentes a estas
        std::vector<std::pair<uint16_t, uint16_t>> adyacent_cells = get_cells_within_radius(cell);

        std::set<Object> objects_set;
        // Recorro las celdas adyacentes
        for (const auto& adyacent_cell: adyacent_cells) {
            // Recorro los objetos de la celda adyacente
            std::vector<Object> const cell_objects =
                    matrix[adyacent_cell.first][adyacent_cell.second];
            for (const auto& obj: cell_objects) {
                objects_set.insert(obj);
            }
        }

        return objects_set;
    }

    std::vector<uint16_t> calculate_new_position(const std::vector<uint16_t>& position,
                                                 const Direction& direction, uint16_t delta,
                                                 uint16_t max_position) {
        /*
         *    Calcula la nueva posición del objeto en función de la dirección y el delta.
         *    Si la nueva posición excede el tamaño máximo, se ajusta a la posición máxima.
         *    Devuelve la nueva posición.
         */

        std::vector<uint16_t> new_position = position;
        switch (direction) {
            case Direction::UP:
                new_position[1] = (position[1] > delta) ? position[1] - delta : 0;
                break;
            case Direction::DOWN:
                new_position[1] =
                        (position[1] + delta > max_position) ? max_position : position[1] + delta;
                break;
            case Direction::LEFT:
                new_position[0] = (position[0] > delta) ? position[0] - delta : 0;
                break;
            case Direction::RIGHT:
                new_position[0] =
                        (position[0] + delta > max_position) ? max_position : position[0] + delta;
                break;
            default:
                break;
        }
        return new_position;
    }

    std::pair<bool, std::vector<uint16_t>> _move(const uint16_t& id, const Direction& direction,
                                                 std::vector<uint16_t>& position, uint16_t size,
                                                 uint16_t delta) {
        /*
         *    Realiza el movimiento del objeto en la dirección especificada.
         *    Devuelve un par que indica si el movimiento fue exitoso y la nueva posición.
         */

        uint16_t MAX_POSITION = MATRIX_SIZE * CELL_SIZE - size;
        std::vector<uint16_t> new_position =
                calculate_new_position(position, direction, delta, MAX_POSITION);

        std::cout << "\tIntentando mover de (" << position[0] << ", " << position[1] << ") a ("
                  << new_position[0] << ", " << new_position[1] << ")" << std::endl;

        if (new_position == position) {
            return {false, position};  // No se movió
        }

        // Obtenemos la posición máxima que se puede mover sin colisionar
        std::vector<uint16_t> max_position = get_max_position(id, position, new_position, size);

        return {max_position != position, max_position};
    }

    std::vector<uint16_t> get_max_position(const uint16_t& id,
                                           const std::vector<uint16_t>& old_position,
                                           const std::vector<uint16_t>& new_position,
                                           const uint16_t& size) {
        /*
         *    Determina la posición máxima a la que se puede mover el objeto sin colisionar.
         */
        std::vector<uint16_t> max_position = old_position;
        auto cell = get_cell_from_position(new_position);

        std::set<Object> ady_objects = get_adyacent_objects(cell);

        // Determinar dirección de movimiento
        int dx = (new_position[0] > old_position[0]) ? 1 :
                 (new_position[0] < old_position[0]) ? -1 :
                                                       0;
        int dy = (new_position[1] > old_position[1]) ? 1 :
                 (new_position[1] < old_position[1]) ? -1 :
                                                       0;

        // Solo se mueve en una dirección a la vez (asumido por el diseño)
        uint16_t x = old_position[0];
        uint16_t y = old_position[1];

        while (x != new_position[0] || y != new_position[1]) {
            uint16_t next_x = x + dx;
            uint16_t next_y = y + dy;
            std::vector<uint16_t> test_position = {next_x, next_y};
            CollisionType collision = collides(test_position, ady_objects, size, id);
            if (collision != CollisionType::NONE) {
                // Si colisiona con una bala, permitimos estar en esa posición pero no avanzar más
                if (collision == CollisionType::BULLET) {
                    max_position = test_position;
                }
                break;
            }
            max_position = test_position;
            x = next_x;
            y = next_y;
        }

        return max_position;
    }

    CollisionType collides(const std::vector<uint16_t>& new_position,
                           const std::set<Object>& objects, uint16_t size, uint16_t id) {
        /*
         *    Verifica si hay colisiones con otros objetos en la nueva posición.
         *    Devuelve el tipo de colisión (BULLET, OTHER o NONE).
         */


        for (const auto& obj: objects) {
            if (id == obj.get_id())
                continue;  // No colisionar con uno mismo
            bool overlap = new_position[0] < obj.get_position()[0] + obj.get_width() &&
                           new_position[0] + size > obj.get_position()[0] &&
                           new_position[1] < obj.get_position()[1] + obj.get_height() &&
                           new_position[1] + size > obj.get_position()[1];
            if (overlap) {
                if (static_cast<ObjectType>(obj.get_type()) == ObjectType::WEAPON)
                    return CollisionType::BULLET;
                else
                    return CollisionType::OTHER;
            }
        }
        return CollisionType::NONE;
    }


    bool update_object_in_matrix(const Object& obj, const std::vector<uint16_t>& old_position) {
        /*
         *    Actualiza la posición del objeto en la matriz.
         *    Elimina el objeto de la celda anterior y lo agrega a la nueva celda.
         */

        auto old_cell = get_cell_from_position(old_position);
        auto new_cell = get_cell_from_position(obj.get_position());

        // Quitar de la celda anterior
        auto& old_vec = matrix[old_cell.first][old_cell.second];
        old_vec.erase(std::remove_if(old_vec.begin(), old_vec.end(),
                                     [id = obj.get_id(), type = obj.get_type()](const Object& o) {
                                         return o.get_id() == id && o.get_type() == type;
                                     }),
                      old_vec.end());

        // Chequear colisión en la nueva celda
        CollisionType collision = collides(obj.get_position(), get_adyacent_objects(new_cell),
                                           obj.get_width(), obj.get_id());

        if (collision == CollisionType::BULLET) {
            // No se agrega a la nueva celda: el objeto "desaparece" del mapa
            std::cout << "\t\t************************************************" << std::endl;
            std::cout << "\t\t******Colisión con bala, objeto eliminado.******" << std::endl;
            std::cout << "\t\t************************************************" << std::endl;
            return false;
        }

        // Si no colisiona con bala, agregar a la nueva celda
        matrix[new_cell.first][new_cell.second].push_back(obj);
        return true;
    }

    void reap() {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                                     [this](const Object& obj) {
                                         auto cell = get_cell_from_position(obj.get_position());
                                         const auto& cell_vec = matrix[cell.first][cell.second];
                                         // Busca si el objeto está en la celda correspondiente de
                                         // la matriz
                                         return std::none_of(cell_vec.begin(), cell_vec.end(),
                                                             [&obj](const Object& o) {
                                                                 return o.get_id() == obj.get_id();
                                                             });
                                     }),
                      objects.end());
    }

public:
    /*
     * Constructor.
     **/

    Game(): matrix(MATRIX_SIZE, std::vector<std::vector<Object>>(MATRIX_SIZE)) {
        initialize_demo_objects();
    }

    bool move(Direction direction, const uint16_t& id) {
        /*
         *    Realiza el movimiento del jugador con el id especificado en la dirección dada.
         */

        // Recolectar objetos que no están en la matriz
        reap();

        auto it = std::find_if(objects.begin(), objects.end(), [id](const Object& obj) {
            return static_cast<ObjectType>(obj.get_type()) == ObjectType::PLAYER &&
                   obj.get_id() == id;
        });
        if (it != objects.end()) {
            std::vector<uint16_t> old_position = it->get_position();
            std::pair<bool, std::vector<uint16_t>> move_result =
                    _move(id, direction, it->get_position(), PLAYER_SIZE, MOVE_STEP);
            if (move_result.first) {
                // Actualizar la posición del objeto
                it->get_position() = move_result.second;

                // Actualizar la posición del objeto en la matriz -> si no lo puede actualizar
                // es porque el objeto "murió" (colisionó con una bala)
                return update_object_in_matrix(*it, old_position);
            }
        } else {
            std::cout << "No se encontró el objeto con ID: " << id << std::endl;
        }
        return false;
    }

    // std::pair<bool, uint16_t> shoot(Direction direction, const uint16_t& id, uint16_t range,
    // uint16_t b_id = 0) {
    //     /*
    //      *    Dispara una bala en la dirección especificada.
    //      *    Devuelve true si se creó la bala, false si no.
    //      */

    //     // Recolectar objetos que no están en la matriz
    //     reap();

    //     auto it = std::find_if(objects.begin(), objects.end(), [id](const Object& obj) {
    //         return obj.type == ObjectType::PLAYER && obj.id == id;
    //     });
    //     if (it != objects.end()) {
    //         Object bullet;
    //         if (b_id == 0) {
    //             position = it->position;
    //             b_id = bullet_id;  // ID de la bala se asigna automáticamente
    //             bullet_id++;
    //         }

    //         bullet.id = b_id;
    //         bullet.type = ObjectType::BULLET;
    //         bullet.position = position;
    //         bullet.width = BULLET_SIZE;
    //         bullet.height = BULLET_SIZE;


    //         // Mover la bala en la dirección especificada
    //         auto move_result = _move(id, direction, bullet.position, BULLET_SIZE, MOVE_STEP);
    //         if (move_result.first) {
    //             bullet.position = move_result.second;
    //             objects.push_back(bullet);
    //             auto cell = get_cell_from_position(bullet.position);
    //             matrix[cell.first][cell.second].push_back(bullet);
    //             return {true, id};
    //         }
    //     } else {
    //         std::cout << "No se encontró el objeto con ID: " << id << std::endl;
    //     }
    //     return {false, 0};
    // }

    std::vector<Object>& get_objects() {
        reap();
        return objects;
    }


    /********************************************************************************************
     ************************************ FUNCIONES PARA TESTEAR ********************************
     ********************************************************************************************/
    void initialize_demo_objects() {
        // Jugador en (10, 10)
        Player player1(1, {10, 10});
        objects.push_back(player1);
        auto cell = get_cell_from_position(player1.get_position());
        matrix[cell.first][cell.second].push_back(player1);

        // Jugador en (60, 5)
        Player player2(2, {60, 5});
        objects.push_back(player2);
        auto cell2 = get_cell_from_position(player2.get_position());
        matrix[cell2.first][cell2.second].push_back(player2);

        // Obstáculo 1 en (0, 10)
        Obstacle obstacle1(6, 6, {0, 10});
        objects.push_back(obstacle1);
        cell = get_cell_from_position(obstacle1.get_position());
        matrix[cell.first][cell.second].push_back(obstacle1);

        // Obstáculo 2 en (90, 0)
        Obstacle obstacle2(10, 10, {90, 0});
        objects.push_back(obstacle2);
        cell = get_cell_from_position(obstacle2.get_position());
        matrix[cell.first][cell.second].push_back(obstacle2);

        // Bala en (50, 10)
        Weapon weapon(0, 10, {50, 10});
        objects.push_back(weapon);
        cell = get_cell_from_position(weapon.get_position());
        matrix[cell.first][cell.second].push_back(weapon);

        // muestro los objetos
        std::cout << "Objetos iniciales:" << std::endl;
        for (const auto& obj: objects) {
            std::cout << "\tObjeto ID: " << obj.get_id()
                      << ", Tipo: " << get_object_type(static_cast<ObjectType>(obj.get_type()))
                      << ", Posición: (" << obj.get_position()[0] << ", " << obj.get_position()[1]
                      << ")" << std::endl;
        }
        std::cout << std::endl;
    }

    std::vector<uint16_t> get_position(uint16_t id) const {
        auto it = std::find_if(objects.begin(), objects.end(),
                               [id](const Object& obj) { return obj.get_id() == id; });
        if (it != objects.end()) {
            return it->get_position();
        }
        return {};
    }

    std::string get_object_type(const ObjectType& type) const {
        switch (type) {
            case ObjectType::PLAYER:
                return "Jugador";
            case ObjectType::WEAPON:
                return "Arma";
            case ObjectType::OBSTACLE:
                return "Obstáculo";
            default:
                return "Desconocido";
        }
    }

    uint16_t distance_moved(const std::vector<uint16_t>& old_position,
                            const std::vector<uint16_t>& new_position) {
        /*
         *    Calcula la distancia recorrida entre dos posiciones.
         */
        return static_cast<uint16_t>(std::sqrt(std::pow(new_position[0] - old_position[0], 2) +
                                               std::pow(new_position[1] - old_position[1], 2)));
    }
};

#endif  // GAME_H
