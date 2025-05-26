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
    enum class CollisionType { BULLET, PLAYER, OBSTACLE, OTHER, NONE };
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
                                                 Direction direction, uint16_t delta,
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

    std::pair<bool, std::vector<uint16_t>> _move(const Object obj, Direction direction,
                                                 uint16_t delta) {
        /*
         *    Realiza el movimiento del objeto dado en la dirección dada.
         *    Devuelve un par que indica si el movimiento fue exitoso y la nueva posición.
         */
        std::vector<uint16_t> position = obj.get_position();

        uint16_t MAX_POSITION = MATRIX_SIZE * CELL_SIZE - obj.get_width();
        std::vector<uint16_t> new_position =
                calculate_new_position(position, direction, delta, MAX_POSITION);
        std::cout << "Intentando mover de (" << position[0] << ", " << position[1] << ") a ("
                  << new_position[0] << ", " << new_position[1] << ")" << std::endl;

        if (new_position == position) {
            return {false, position};  // No se movió
        }

        std::vector<uint16_t> max_position = get_max_position(obj, new_position);

        return {max_position != position, max_position};
    }

    std::vector<uint16_t> get_max_position(const Object& obj,
                                           const std::vector<uint16_t>& new_position) {
        /*
         *    Determina la posición máxima a la que se puede mover el objeto sin colisionar.
         */
        std::vector<uint16_t> max_position = obj.get_position();
        auto cell = get_cell_from_position(new_position);
        std::set<Object> ady_objects = get_adyacent_objects(cell);

        // Determinar dirección de movimiento
        int dx = (new_position[0] > max_position[0]) ? 1 :
                 (new_position[0] < max_position[0]) ? -1 :
                                                       0;
        int dy = (new_position[1] > max_position[1]) ? 1 :
                 (new_position[1] < max_position[1]) ? -1 :
                                                       0;

        // Solo se mueve en una dirección a la vez (asumido por el diseño)
        uint16_t x = max_position[0];
        uint16_t y = max_position[1];

        while (x != new_position[0] || y != new_position[1]) {
            uint16_t next_x = x + dx;
            uint16_t next_y = y + dy;
            std::vector<uint16_t> test_position = {next_x, next_y};
            std::cout << "\t\tProbando posición: (" << test_position[0] << ", " << test_position[1]
                      << ")" << std::endl;
            CollisionType collision = collides(obj, test_position, ady_objects);
            if (collision != CollisionType::NONE) {
                ObjectType obj_type = static_cast<ObjectType>(obj.get_type());
                if (collision == CollisionType::BULLET && obj_type == ObjectType::PLAYER) {
                    // Si un jugador colisiona con una bala, se mueve hasta la posición de la bala
                    max_position = test_position;
                    std::cout << "\t\tJugador " << obj.get_id() << " colisionó con una bala."
                              << std::endl;
                } else if (collision == CollisionType::PLAYER && obj_type == ObjectType::WEAPON) {
                    // Si una bala colisiona con un jugador, se mueve hasta la posición del jugador
                    max_position = test_position;
                    std::cout << "\t\tBala colisionó con un jugador." << std::endl;
                    /*
                        TODO: chequear si el jugador muere y eliminarlo de la matriz o restarle vida
                    */
                } else if (collision == CollisionType::OBSTACLE && obj_type == ObjectType::WEAPON) {
                    // Si una bala colisiona con un obstáculo, se mueve hasta la posición del
                    // obstáculo
                    max_position = test_position;
                    std::cout << "\t\tBala colisionó con un obstáculo." << std::endl;
                }
                break;
                std::cout << "\t\tColisión detectada" << std::endl;
            }
            max_position = test_position;
            x = next_x;
            y = next_y;
        }

        return max_position;
    }

    CollisionType collides(const Object& object, const std::vector<uint16_t>& new_position,
                           const std::set<Object>& objects) {
        /*
         *    Verifica si hay colisiones con otros objetos en la nueva posición.
         *    Devuelve el tipo de colisión (BULLET, PLAYER, OBSTACLE, OTHER o NONE).
         */

        uint16_t width = object.get_width();
        uint16_t height = object.get_height();
        for (const Object& obj: objects) {
            if (object == obj) {
                continue;
            }
            bool overlap = new_position[0] < obj.get_position()[0] + obj.get_width() &&
                           new_position[0] + width > obj.get_position()[0] &&
                           new_position[1] < obj.get_position()[1] + obj.get_height() &&
                           new_position[1] + height > obj.get_position()[1];
            if (overlap) {
                std::cout << "\tColisión detectada entre objeto ID: " << object.get_id()
                          << " y objeto ID: " << obj.get_id() << std::endl;
                switch (static_cast<ObjectType>(
                        obj.get_type()))  // Cast to ObjectType for comparison
                {
                    case ObjectType::WEAPON:
                        std::cout << "\tColisión con arma detectada." << std::endl;
                        return CollisionType::BULLET;
                    case ObjectType::OBSTACLE:
                        std::cout << "\tColisión con obstáculo detectada." << std::endl;
                        return CollisionType::OBSTACLE;
                    case ObjectType::PLAYER:
                        std::cout << "\tColisión con jugador " << obj.get_id() << " detectada."
                                  << std::endl;
                        return CollisionType::PLAYER;
                    default:
                        return CollisionType::OTHER;
                }
            }
        }
        return CollisionType::NONE;
    }


    bool update_object_in_matrix(Object* obj, const std::vector<uint16_t>& old_position) {
        /*
         *    Actualiza la posición del objeto en la matriz.
         *    Elimina el objeto de la celda anterior y lo agrega a la nueva celda.
         */

        auto old_cell = get_cell_from_position(old_position);
        auto new_cell = get_cell_from_position(obj->get_position());

        // Quitar de la celda anterior
        auto& old_vec = matrix[old_cell.first][old_cell.second];
        old_vec.erase(std::remove_if(old_vec.begin(), old_vec.end(),
                                     [obj](const Object& o) { return o == *obj; }),
                      old_vec.end());

        // Chequear colisión en la nueva celda
        CollisionType collision =
                collides(*obj, obj->get_position(), get_adyacent_objects(new_cell));

        if (collision != CollisionType::NONE) {
            // Si colisiona con un obstáculo, no se mueve
            if (collision == CollisionType::OBSTACLE) {
                std::cout << "Objeto " << obj->get_id()
                          << " colisionó con un obstáculo y no se movió." << std::endl;
                return false;
            }
        }

        if (collision == CollisionType::BULLET &&
            static_cast<ObjectType>(obj->get_type()) == ObjectType::PLAYER) {
            /*
                TODO: chequear si el jugador muere y eliminarlo de la matriz o restarle vida
            */
            std::cout << "Jugador " << obj->get_id() << " colisionó con una bala y murió."
                      << std::endl;
            return false;
        }

        // Si no colisiona con bala, agregar a la nueva celda
        matrix[new_cell.first][new_cell.second].push_back(*obj);
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

    void initialize_demo_objects() {
        // Obstáculo 1 en (50, 10)
        Obstacle obstacle1(6, 6, {50, 10});
        objects.push_back(obstacle1);
        auto cell = get_cell_from_position(obstacle1.get_position());
        matrix[cell.first][cell.second].push_back(obstacle1);
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
            std::pair<bool, std::vector<uint16_t>> move_result = _move(*it, direction, MOVE_STEP);
            if (move_result.first) {
                // Actualizar la posición del objeto
                it->move(move_result.second);

                // Actualizar la posición del objeto en la matriz -> si no lo puede actualizar
                // es porque el objeto "murió" (colisionó con una bala)
                return update_object_in_matrix(&(*it), old_position);
            }
        } else {
            std::cout << "No se encontró el objeto con ID: " << id << std::endl;
        }
        return false;
    }

    std::vector<Object>& get_objects() {
        reap();
        return objects;
    }

    void add_player(const uint16_t& id) {
        Player player1(id, {0, 0});
        objects.push_back(player1);
        auto cell = get_cell_from_position(player1.get_position());
        matrix[cell.first][cell.second].push_back(player1);
    }
};

#endif  // GAME_H
