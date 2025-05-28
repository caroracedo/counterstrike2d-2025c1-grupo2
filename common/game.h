#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 200   // posición máxima en la matriz
#define MOVE_STEP 5       // paso de movimiento
#define CELL_SIZE 56      // tamaño de cada celda en la matriz
#define PLAYER_SIZE 32    // tamaño del jugador
#define BULLET_SIZE 8     // tamaño de la bala
#define WEAPON_DAMAGE 20  // daño del arma

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
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
    std::vector<std::vector<std::vector<std::shared_ptr<Object>>>> matrix;
    std::vector<std::shared_ptr<Object>> objects;
    enum class CollisionType { BULLET, PLAYER, OBSTACLE, OTHER, NONE };
    uint16_t bullet_id = 1;

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

    std::set<std::shared_ptr<Object>> get_adyacent_objects(
            const std::pair<uint16_t, uint16_t>& cell) {
        /*
         *    Obtiene los objetos en las celdas adyacentes a la celda dada
         */

        // Vector con todas las celdas adyacentes y las adyacentes a estas
        std::vector<std::pair<uint16_t, uint16_t>> adyacent_cells = get_cells_within_radius(cell);

        std::set<std::shared_ptr<Object>> objects_set;
        // Recorro las celdas adyacentes
        for (const auto& adyacent_cell: adyacent_cells) {
            // Recorro los objetos de la celda adyacente
            const auto& cell_objects = matrix[adyacent_cell.first][adyacent_cell.second];
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

    std::pair<bool, std::vector<uint16_t>> _move(const std::shared_ptr<Object>& obj,
                                                 Direction direction, uint16_t delta) {
        /*
         *    Realiza el movimiento del objeto dado en la dirección dada.
         *    Realiza el movimiento del objeto dado en la dirección dada.
         *    Devuelve un par que indica si el movimiento fue exitoso y la nueva posición.
         */
        std::vector<uint16_t> position = obj->get_position();

        uint16_t MAX_POSITION = MATRIX_SIZE * CELL_SIZE - obj->get_width();
        std::vector<uint16_t> new_position =
                calculate_new_position(position, direction, delta, MAX_POSITION);

        std::cout << "\nIntentando mover "
                  << get_object_type(static_cast<ObjectType>(obj->get_type())) << " con ID "
                  << obj->get_id() << " en dirección " << direction_to_string(direction)
                  << " hacia (" << new_position[0] << ", " << new_position[1] << ")" << std::endl;

        if (new_position == position) {
            return {false, position};  // No se movió
        }

        std::vector<uint16_t> max_position = get_max_position(obj, new_position);

        return {max_position != position, max_position};
    }

    std::vector<uint16_t> get_max_position(const std::shared_ptr<Object>& obj,
                                           const std::vector<uint16_t>& new_position) {
        /*
         *    Determina la posición máxima a la que se puede mover el objeto sin colisionar.
         */
        std::vector<uint16_t> max_position = obj->get_position();
        auto cell = get_cell_from_position(new_position);
        std::set<std::shared_ptr<Object>> ady_objects = get_adyacent_objects(cell);

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
            auto collision_result = collides(obj, test_position, ady_objects);
            ObjectType collision = static_cast<ObjectType>(collision_result.first);
            if (collision != ObjectType::UNKNOWN) {
                ObjectType obj_type = static_cast<ObjectType>(obj->get_type());
                if (collision == ObjectType::WEAPON && obj_type == ObjectType::PLAYER) {
                    // Si un jugador colisiona con una bala, se mueve hasta la posición de la bala
                    max_position = test_position;

                } else if (collision == ObjectType::PLAYER && obj_type == ObjectType::WEAPON) {
                    // Si una bala colisiona con un jugador, se mueve hasta la posición del jugador
                    max_position = test_position;

                } else if (collision == ObjectType::OBSTACLE && obj_type == ObjectType::WEAPON) {
                    // Si una bala colisiona con un obstáculo, se mueve hasta la posición del
                    // obstáculo
                    max_position = test_position;
                }
                std::cout << "\tColisión detectada" << std::endl;
                break;
            }
            max_position = test_position;
            x = next_x;
            y = next_y;
        }

        return max_position;
    }

    std::pair<ObjectType, uint16_t> collides(const std::shared_ptr<Object>& object,
                                             const std::vector<uint16_t>& new_position,
                                             const std::set<std::shared_ptr<Object>>& objects) {
        /*
         *    Verifica si hay colisiones con otros objetos en la nueva posición.
         *    Devuelve el tipo de colisión (BULLET, PLAYER, OBSTACLE, OTHER o NONE).
         *    Devuelve el tipo de colisión (BULLET, PLAYER, OBSTACLE, OTHER o NONE).
         */

        uint16_t width = object->get_width();
        uint16_t height = object->get_height();
        for (const auto& obj: objects) {
            if (object == obj) {
                continue;
            }
            bool overlap = new_position[0] < obj->get_position()[0] + obj->get_width() &&
                           new_position[0] + width > obj->get_position()[0] &&
                           new_position[1] < obj->get_position()[1] + obj->get_height() &&
                           new_position[1] + height > obj->get_position()[1];
            if (overlap) {
                switch (static_cast<ObjectType>(
                        obj->get_type()))  // Cast to ObjectType for comparison
                {
                    case ObjectType::WEAPON:

                        return {ObjectType::WEAPON, obj->get_id()};
                    case ObjectType::OBSTACLE:

                        return {ObjectType::OBSTACLE, obj->get_id()};
                    case ObjectType::PLAYER:

                        return {ObjectType::PLAYER, obj->get_id()};
                    default:
                        return {ObjectType::UNKNOWN, 0};  // No se reconoce el tipo de objeto
                }
            }
        }
        return {ObjectType::UNKNOWN, 0};  // No hay colisión
    }


    bool update_object_in_matrix(const std::shared_ptr<Object>& obj,
                                 const std::vector<uint16_t>& old_position) {
        /*
         *    Actualiza la posición del objeto en la matriz.
         *    Elimina el objeto de la celda anterior y lo agrega a la nueva celda.
         */

        auto old_cell = get_cell_from_position(old_position);
        auto new_cell = get_cell_from_position(obj->get_position());

        // Quitar de la celda anterior
        auto& old_vec = matrix[old_cell.first][old_cell.second];
        old_vec.erase(std::remove_if(old_vec.begin(), old_vec.end(),
                                     [obj](const std::shared_ptr<Object>& o) { return o == obj; }),
                      old_vec.end());

        // Chequear colisión en la nueva celda
        auto collision_result = collides(obj, obj->get_position(), get_adyacent_objects(new_cell));

        ObjectType collider_type = static_cast<ObjectType>(collision_result.first);

        if (collider_type == ObjectType::WEAPON &&
            static_cast<ObjectType>(obj->get_type()) == ObjectType::PLAYER) {
            obj->take_damage(WEAPON_DAMAGE);
            std::cout << "Jugador " << obj->get_id() << " colisionó con una bala" << std::endl;
            if (!obj->is_alive()) {
                return false;
            }
        }

        if (collider_type == ObjectType::PLAYER &&
            static_cast<ObjectType>(obj->get_type()) == ObjectType::WEAPON) {
            std::cout << "\tBala colisionó con un jugador" << std::endl;

            uint16_t player_id = collision_result.second;
            find_player_and_damage(player_id);

            return false;  // La bala no se mueve si colisiona con un jugador
        }

        // Si no colisiona con bala, agregar a la nueva celda
        matrix[new_cell.first][new_cell.second].push_back(obj);
        return true;
    }

    void find_player_and_damage(uint16_t id) {
        auto player_it = std::find_if(
                objects.begin(), objects.end(), [id](const std::shared_ptr<Object>& o) {
                    return static_cast<ObjectType>(o->get_type()) == ObjectType::PLAYER &&
                           o->get_id() == id;
                });
        if (player_it != objects.end()) {
            (*player_it)->take_damage(WEAPON_DAMAGE);
            if (!(*player_it)->is_alive()) {
                std::cout << "\t\tJugador " << (*player_it)->get_id() << " ha muerto." << std::endl;
                // Eliminar el jugador de la matriz y de los objetos
                auto cell = get_cell_from_position((*player_it)->get_position());
                auto& cell_vec = matrix[cell.first][cell.second];
                cell_vec.erase(std::remove_if(cell_vec.begin(), cell_vec.end(),
                                              [id](const std::shared_ptr<Object>& o) {
                                                  return o->get_id() == id;
                                              }),
                               cell_vec.end());
                objects.erase(player_it);
            }
        }
    }

    void reap() {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                                     [this](const std::shared_ptr<Object>& obj) {
                                         auto cell = get_cell_from_position(obj->get_position());
                                         const auto& cell_vec = matrix[cell.first][cell.second];
                                         // Busca si el objeto está en la celda correspondiente de
                                         // la matriz
                                         return std::none_of(
                                                 cell_vec.begin(), cell_vec.end(),
                                                 [&obj](const std::shared_ptr<Object>& o) {
                                                     return o == obj;
                                                 });
                                     }),
                      objects.end());
    }

    void inc_bullet_id() {
        /*
         *    Incrementa el ID de la bala para que no se repita.
         */
        bullet_id++;
        if (bullet_id == 0) {
            bullet_id = 1;  // Evitar overflow
        }
    }

    std::vector<uint16_t> calculate_bullet_starting_position(
            const Direction& direction, const std::shared_ptr<Object>& player) {
        /*
         *    Calcula la posición inicial de la bala en función de la dirección y la posición del
         * jugador. La bala se coloca en el centro del jugador, desplazada según la dirección.
         */
        std::vector<uint16_t> position = player->get_position();
        uint16_t offset = PLAYER_SIZE / 2 - BULLET_SIZE / 2;  // Centrar la bala en el jugador

        switch (direction) {
            case Direction::UP:
                position[1] -= offset;
                break;
            case Direction::DOWN:
                position[1] += offset;
                break;
            case Direction::LEFT:
                position[0] -= offset;
                break;
            case Direction::RIGHT:
                position[0] += offset;
                break;
            default:
                break;
        }
        return position;
    }

public:
    /*
     * Constructor.
     **/

    Game(): matrix(MATRIX_SIZE, std::vector<std::vector<std::shared_ptr<Object>>>(MATRIX_SIZE)) {
        initialize_demo_objects();
    }

    bool move(Direction direction, const uint16_t& id) {
        /*
         *    Realiza el movimiento del jugador con el id especificado en la dirección dada.
         */

        // Recolectar objetos que no están en la matriz
        reap();

        auto it = std::find_if(
                objects.begin(), objects.end(), [id](const std::shared_ptr<Object>& obj) {
                    return static_cast<ObjectType>(obj->get_type()) == ObjectType::PLAYER &&
                           obj->get_id() == id;
                });
        if (it != objects.end()) {
            std::vector<uint16_t> old_position = (*it)->get_position();
            std::pair<bool, std::vector<uint16_t>> move_result = _move(*it, direction, MOVE_STEP);
            if (move_result.first) {
                // Actualizar la posición del objeto
                (*it)->move(move_result.second);

                // Actualizar la posición del objeto en la matriz -> si no lo puede actualizar
                // es porque el objeto "murió" (colisionó con una bala)
                return update_object_in_matrix(*it, old_position);
            }
        } else {
            std::cout << "No se encontró el objeto con ID: " << id << std::endl;
        }
        return false;
    }

    void update_bullets() {
        // busca todas las balas vivas y las mueve otro poco
    }

    // POSICION IDEAL (DONDE APUNTA EL MOUSE), ID JUGADOR -> get_current_weapon()
    // jugador->current_weapon->bala nueva y la agrego a la matriz
    std::pair<bool, uint16_t> shoot(Direction direction, const uint16_t& id, uint16_t range,
                                    uint16_t b_id = 0) {
        /*
         *    Dispara una bala en la dirección especificada.
         *    Devuelve true si se creó la bala, false si no y el id de la bala.
         */

        // Recolectar objetos que no están en la matriz
        reap();

        if (b_id == 0) {
            // Si no se proporciona un ID de bala, se crea una nueva bala
            auto it = std::find_if(
                    objects.begin(), objects.end(), [id](const std::shared_ptr<Object>& obj) {
                        return static_cast<ObjectType>(obj->get_type()) == ObjectType::PLAYER &&
                               obj->get_id() == id;
                    });
            if (it != objects.end()) {
                b_id = bullet_id;
                inc_bullet_id();

                std::vector<uint16_t> position = calculate_bullet_starting_position(direction, *it);

                auto weapon = std::make_shared<Weapon>(b_id, range, position);
                objects.push_back(weapon);
                auto cell = get_cell_from_position(weapon->get_position());
                matrix[cell.first][cell.second].push_back(weapon);
            }
        }

        auto it = std::find_if(
                objects.begin(), objects.end(), [b_id](const std::shared_ptr<Object>& obj) {
                    return static_cast<ObjectType>(obj->get_type()) == ObjectType::WEAPON &&
                           obj->get_id() == b_id;
                });
        if (it != objects.end()) {
            (*it)->set_range(range);
            std::vector<uint16_t> old_position = (*it)->get_position();
            std::pair<bool, std::vector<uint16_t>> move_result = _move(*it, direction, MOVE_STEP);
            if (move_result.first) {
                // Actualizar la posición del objeto
                (*it)->move(move_result.second);

                // Actualizar la posición del objeto en la matriz -> si no lo puede actualizar
                // es porque el objeto "murió" (colisionó con un obstáculo o jugador)
                return {update_object_in_matrix(*it, old_position), b_id};
            }
        } else {
            std::cout << "No se encontró el objeto con ID de bala: " << b_id << std::endl;
            return {false, 0};  // No se pudo disparar
        }
        return {false, 0};
    }


    std::vector<std::shared_ptr<Object>>& get_objects() {
        reap();
        return objects;
    }

    void add_player(const uint16_t& id) {
        auto player1 = std::make_shared<Player>(id, std::vector<uint16_t>{0, 0});
        objects.push_back(player1);
        auto cell = get_cell_from_position(player1->get_position());
        matrix[cell.first][cell.second].push_back(player1);
    }

    /********************************************************************************************
     ************************************ FUNCIONES PARA TESTEAR ********************************
     ********************************************************************************************/
    void initialize_demo_objects() {
        // Jugador en (10, 10)
        auto player1 = std::make_shared<Player>(1, std::vector<uint16_t>{10, 10});
        objects.push_back(player1);
        auto cell = get_cell_from_position(player1->get_position());
        matrix[cell.first][cell.second].push_back(player1);

        // Obstáculo 1 en (50, 10)
        auto obstacle1 = std::make_shared<Obstacle>(6, 6, std::vector<uint16_t>{50, 10});
        objects.push_back(obstacle1);
        cell = get_cell_from_position(obstacle1->get_position());
        matrix[cell.first][cell.second].push_back(obstacle1);

        // Bala en (50, 50)
        auto weapon = std::make_shared<Weapon>(0, 10, std::vector<uint16_t>{45, 50});
        objects.push_back(weapon);
        cell = get_cell_from_position(weapon->get_position());
        matrix[cell.first][cell.second].push_back(weapon);

        auto player2 = std::make_shared<Player>(2, std::vector<uint16_t>{80, 80});
        objects.push_back(player2);
        cell = get_cell_from_position(player2->get_position());
        matrix[cell.first][cell.second].push_back(player2);

        auto player3 = std::make_shared<Player>(3, std::vector<uint16_t>{30, 80});
        objects.push_back(player3);
        cell = get_cell_from_position(player3->get_position());
        matrix[cell.first][cell.second].push_back(player3);
    }

    void show_objects() {
        std::cout << "Objetos:" << std::endl;
        for (const auto& obj: objects) {
            std::cout << "\tObjeto ID: " << obj->get_id()
                      << ", Tipo: " << get_object_type(static_cast<ObjectType>(obj->get_type()))
                      << ", Posición: (" << obj->get_position()[0] << ", " << obj->get_position()[1]
                      << ")" << std::endl;
        }
        std::cout << std::endl;
    }

    std::vector<uint16_t> get_position(uint16_t id) const {
        auto it = std::find_if(
                objects.begin(), objects.end(),
                [id](const std::shared_ptr<Object>& obj) { return obj->get_id() == id; });
        if (it != objects.end()) {
            return (*it)->get_position();
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

    std::string direction_to_string(Direction direction) const {
        /*
         *    Convierte la dirección a una cadena de texto.
         */
        switch (direction) {
            case Direction::UP:
                return "arriba";
            case Direction::DOWN:
                return "abajo";
            case Direction::LEFT:
                return "izquierda";
            case Direction::RIGHT:
                return "derecha";
            default:
                return "desconocida";
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
