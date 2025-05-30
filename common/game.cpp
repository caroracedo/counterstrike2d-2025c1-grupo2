#include "game.h"

#include <iostream>

Game::Game(): matrix(MATRIX_SIZE, std::vector<std::vector<std::shared_ptr<Object>>>(MATRIX_SIZE)) {
    initialize_demo_objects();
}

std::pair<uint16_t, uint16_t> Game::get_cell_from_position(const std::vector<uint16_t>& position) {
    uint16_t x = position[0] / CELL_SIZE;
    uint16_t y = position[1] / CELL_SIZE;
    if (x >= MATRIX_SIZE)
        x = MATRIX_SIZE - 1;
    if (y >= MATRIX_SIZE)
        y = MATRIX_SIZE - 1;
    return {x, y};
}

std::vector<std::pair<uint16_t, uint16_t>> Game::get_cells_within_radius(
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

std::set<std::shared_ptr<Object>> Game::get_adyacent_objects(
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

std::pair<bool, std::vector<uint16_t>> Game::_move(const Object& obj,
                                                   std::vector<uint16_t> new_position) {
    /*
     *    Realiza el movimiento del objeto dado en la dirección dada.
     *    Realiza el movimiento del objeto dado en la dirección dada.
     *    Devuelve un par que indica si el movimiento fue exitoso y la nueva posición.
     */

    std::vector<uint16_t> position = obj.get_position();

    std::cout << "\nIntentando mover " << get_object_type(static_cast<ObjectType>(obj.get_type()))
              << " con ID " << obj.get_id() << " desde (" << position[0] << ", " << position[1]
              << ")"
              << " hacia (" << new_position[0] << ", " << new_position[1] << ")" << std::endl;

    if (new_position == position) {
        return {false, position};  // No se puede mover
    }

    std::vector<uint16_t> max_position = get_max_position(obj, new_position);

    return {max_position != position, max_position};
}

std::vector<uint16_t> Game::get_max_position(const Object& obj,
                                             const std::vector<uint16_t>& new_position) {
    /*
     *    Determina la posición máxima a la que se puede mover el objeto sin colisionar.
     */
    std::vector<uint16_t> max_position = obj.get_position();
    auto cell = get_cell_from_position(new_position);
    std::set<std::shared_ptr<Object>> ady_objects = get_adyacent_objects(cell);

    int dx = (new_position[0] > max_position[0]) ? 1 : (new_position[0] < max_position[0]) ? -1 : 0;
    int dy = (new_position[1] > max_position[1]) ? 1 : (new_position[1] < max_position[1]) ? -1 : 0;

    uint16_t x = max_position[0];
    uint16_t y = max_position[1];

    int max_steps = 10;  // Límite de seguridad

    while ((x != new_position[0] || y != new_position[1]) && max_steps-- > 0) {
        uint16_t next_x = x + dx;
        uint16_t next_y = y + dy;
        std::vector<uint16_t> test_position = {next_x, next_y};
        auto collision_result = collides(obj, test_position, ady_objects);
        ObjectType collision = static_cast<ObjectType>(collision_result.first);
        if (collision != ObjectType::UNKNOWN) {
            ObjectType obj_type = static_cast<ObjectType>(obj.get_type());
            if (collision == ObjectType::BULLET && obj_type == ObjectType::PLAYER) {
                max_position = test_position;
            } else if (collision == ObjectType::PLAYER && obj_type == ObjectType::BULLET) {
                max_position = test_position;
            } else if (collision == ObjectType::OBSTACLE && obj_type == ObjectType::BULLET) {
                max_position = test_position;
            }
            std::cout << "\tColisión detectada entre " << get_object_type(collision) << " "
                      << collision_result.second << " y " << get_object_type(obj.get_type()) << " "
                      << obj.get_id() << std::endl;
            break;
        }
        max_position = test_position;
        x = next_x;
        y = next_y;
    }

    return max_position;
}

std::pair<ObjectType, uint16_t> Game::collides(const Object& object,
                                               const std::vector<uint16_t>& new_position,
                                               const std::set<std::shared_ptr<Object>>& objects) {
    /*
     *    Verifica si hay colisiones con otros objetos en la nueva posición.
     *    Devuelve el tipo de objeto con el que colisiona y su ID.
     */

    uint16_t width = object.get_width();
    uint16_t height = object.get_height();
    for (const auto& obj: objects) {
        if (object == *obj) {
            continue;
        }
        bool overlap = new_position[0] < obj->get_position()[0] + obj->get_width() &&
                       new_position[0] + width > obj->get_position()[0] &&
                       new_position[1] < obj->get_position()[1] + obj->get_height() &&
                       new_position[1] + height > obj->get_position()[1];
        if (overlap) {

            ObjectType type = static_cast<ObjectType>(obj->get_type());
            if (type == ObjectType::BULLET || type == ObjectType::OBSTACLE ||
                type == ObjectType::PLAYER) {
                return {type, obj->get_id()};
            }
            return {ObjectType::UNKNOWN, 0};
        }
    }
    return {ObjectType::UNKNOWN, 0};  // No hay colisión
}

bool Game::update_object_in_matrix(const std::shared_ptr<Object>& obj,
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
    auto collision_result = collides(*obj, obj->get_position(), get_adyacent_objects(new_cell));

    ObjectType collider_type = static_cast<ObjectType>(collision_result.first);
    uint16_t collider_id = collision_result.second;

    if (collider_type == ObjectType::BULLET &&
        static_cast<ObjectType>(obj->get_type()) == ObjectType::PLAYER) {
        std::cout << "\tJugador colisionó con una bala" << std::endl;

        uint16_t damage = get_damage_and_delete_bullet(collider_id);

        bool player_is_alive = find_player_and_damage(obj->get_id(), damage);

        if (!player_is_alive) {
            return false;
        }  // El jugador murió, no se mueve
    }

    if (collider_type == ObjectType::PLAYER &&
        static_cast<ObjectType>(obj->get_type()) == ObjectType::BULLET) {
        std::cout << "\tBala colisionó con un jugador" << std::endl;

        uint16_t damage = get_damage_and_delete_bullet(obj->get_id());

        uint16_t player_id = collision_result.second;
        find_player_and_damage(player_id, damage);

        return false;  // La bala no se mueve si colisiona con un jugador
    }

    if (collider_type == ObjectType::OBSTACLE &&
        static_cast<ObjectType>(obj->get_type()) == ObjectType::BULLET) {
        std::cout << "\tBala colisionó con un obstáculo" << std::endl;

        get_damage_and_delete_bullet(obj->get_id());

        // No se inflige daño a los obstáculos, solo se elimina la bala
        return false;  // La bala no se mueve si colisiona con un obstáculo
    }

    // Si no se muere, agregar a la nueva celda
    matrix[new_cell.first][new_cell.second].push_back(obj);
    return true;
}

uint16_t Game::get_damage_and_delete_bullet(uint bullet_id) {
    uint16_t damage = 0;
    auto bullet_it = bullets.find(bullet_id);
    if (bullet_it != bullets.end()) {
        damage = bullet_it->second->get_damage();

        // Guarda la posición y el id antes de borrar
        auto bullet_pos = bullet_it->second->get_position();
        uint bullet_id_val = bullet_it->first;

        // Eliminar la bala de la lista de balas
        bullets.erase(bullet_it);

        // Eliminar la bala de la matriz
        auto bullet_cell = get_cell_from_position(bullet_pos);
        auto& bullet_vec = matrix[bullet_cell.first][bullet_cell.second];
        bullet_vec.erase(std::remove_if(bullet_vec.begin(), bullet_vec.end(),
                                        [bullet_id_val](const std::shared_ptr<Object>& o) {
                                            return o->get_id() == bullet_id_val &&
                                                   static_cast<ObjectType>(o->get_type()) ==
                                                           ObjectType::BULLET;
                                        }),
                         bullet_vec.end());

        // Eliminar la bala de los objetos
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                                     [bullet_id_val](const std::shared_ptr<Object>& o) {
                                         return o->get_id() == bullet_id_val &&
                                                static_cast<ObjectType>(o->get_type()) ==
                                                        ObjectType::BULLET;
                                     }),
                      objects.end());
    }

    return damage;
}

bool Game::find_player_and_damage(uint16_t id, uint16_t damage) {
    /*
        Inflige daño al jugador con el ID especificado y devuelve si sigue vivo.
        Si el jugador muere, lo elimina de la lista de jugadores, de la matriz y de los objetos.
    */
    auto player_it = players.find(id);
    if (player_it != players.end() && damage > 0) {
        // Infligir daño al jugador
        player_it->second->take_damage(damage);
        std::cout << "\tJugador " << id << " recibió " << damage
                  << " de daño por colisión con bala." << std::endl;

        if (!player_it->second->is_alive()) {
            std::cout << "\tJugador " << id << " ha muerto." << std::endl;

            // Guarda la posición antes de borrar
            auto player_pos = player_it->second->get_position();

            // Eliminar al jugador de players
            players.erase(player_it);

            // Ahora usa player_pos en vez de player_it
            auto player_cell = get_cell_from_position(player_pos);
            auto& player_vec = matrix[player_cell.first][player_cell.second];
            player_vec.erase(std::remove_if(player_vec.begin(), player_vec.end(),
                                            [id](const std::shared_ptr<Object>& o) {
                                                return o->get_id() == id &&
                                                       static_cast<ObjectType>(o->get_type()) ==
                                                               ObjectType::PLAYER;
                                            }),
                             player_vec.end());

            // Eliminar al jugador de los objetos
            objects.erase(std::remove_if(objects.begin(), objects.end(),
                                         [id](const std::shared_ptr<Object>& o) {
                                             return o->get_id() == id &&
                                                    static_cast<ObjectType>(o->get_type()) ==
                                                            ObjectType::PLAYER;
                                         }),
                          objects.end());

            return false;  // El jugador ha muerto
        }
        return true;  // El jugador sigue vivo
    }
    return false;  // No se encontró el jugador o no se infligió daño
}

void Game::inc_bullet_id() {
    /*
     *    Incrementa el ID de la bala para que no se repita.
     */
    bullet_id++;
    if (bullet_id == 0) {
        bullet_id = 1;  // Evitar overflow
    }
}

std::vector<uint16_t> Game::calculate_bullet_starting_position(
        const std::vector<uint16_t>& player_position,
        const std::vector<uint16_t>& desired_position) {
    // Centro del jugador
    int x_jugador = player_position[0] + PLAYER_SIZE / 2;
    int y_jugador = player_position[1] + PLAYER_SIZE / 2;

    int dx = static_cast<int>(desired_position[0]) - x_jugador;
    int dy = static_cast<int>(desired_position[1]) - y_jugador;

    if (dx == 0 && dy == 0) {
        // Disparo en el mismo lugar, poner la bala justo arriba del jugador
        return {static_cast<uint16_t>(x_jugador), static_cast<uint16_t>(y_jugador)};
    }

    // Offset: desde el centro del jugador hasta el borde, más la mitad de la bala
    int offset = PLAYER_SIZE / 2 + BULLET_SIZE / 2;

    // Normaliza la dirección usando el máximo común divisor
    int gcd = std::gcd(std::abs(dx), std::abs(dy));
    int step_dx = (gcd == 0) ? 0 : dx / gcd;
    int step_dy = (gcd == 0) ? 0 : dy / gcd;

    // Posición inicial alineada a múltiplos de 5
    int x_bala = x_jugador + step_dx * offset;
    int y_bala = y_jugador + step_dy * offset;

    // Redondea hacia el múltiplo de 5 más cercano
    x_bala = (x_bala / 5) * 5;
    y_bala = (y_bala / 5) * 5;

    int max_pos = MATRIX_SIZE * CELL_SIZE - BULLET_SIZE;
    x_bala = std::max(0, std::min(x_bala, max_pos));
    y_bala = std::max(0, std::min(y_bala, max_pos));

    return {static_cast<uint16_t>(x_bala), static_cast<uint16_t>(y_bala)};
}

bool Game::move(Direction direction, const uint16_t& id) {
    /*
     *    Realiza el movimiento del jugador con el id especificado en la dirección dada.
     */

    // Buscar el jugador con el ID especificado
    auto player_it = players.find(id);
    if (player_it != players.end()) {
        std::vector<uint16_t> old_position = player_it->second->get_position();
        std::vector<uint16_t> new_position = player_it->second->get_next_position(direction);
        std::pair<bool, std::vector<uint16_t>> move_result =
                _move(static_cast<Object>(*(player_it->second)), new_position);
        if (move_result.first) {
            // Actualizar la posición del objeto
            player_it->second->move(move_result.second);

            // Actualizar la posición del objeto en la matriz -> si no lo puede actualizar
            // es porque el objeto "murió" (colisionó con una bala)
            return update_object_in_matrix(std::static_pointer_cast<Object>(player_it->second),
                                           old_position) ||
                   old_position != move_result.second;
        }
    } else {
        std::cout << "No se encontró el jugador con ID: " << id << std::endl;
        return false;  // No se encontró el jugador
    }
    return false;
}

void Game::update_bullets() {
    std::vector<uint16_t> bullets_to_update;
    std::vector<uint16_t> bullets_to_delete;

    // Clasifica las balas
    for (const auto& [id, bullet]: bullets) {
        if (bullet->is_alive()) {
            auto move_result = _move(*bullet, bullet->get_next_position());
            if (move_result.first) {
                bullets_to_update.push_back(id);
            } else {
                bullets_to_delete.push_back(id);
            }
        } else {
            bullets_to_delete.push_back(id);
        }
    }

    // Actualiza las balas vivas que pueden moverse
    for (auto id: bullets_to_update) {
        auto bullet_it = bullets.find(id);
        if (bullet_it != bullets.end()) {
            auto bullet = bullet_it->second;
            std::vector<uint16_t> old_position = bullet->get_position();
            bullet->move(bullet->get_next_position());
            update_object_in_matrix(bullet, old_position);
        }
    }

    // Elimina las balas muertas o que no pueden moverse
    for (auto id: bullets_to_delete) {
        get_damage_and_delete_bullet(id);
    }
}

bool Game::shoot(const std::vector<uint16_t>& desired_position, const uint16_t& player_id) {
    auto player_it = players.find(player_id);
    if (player_it != players.end()) {
        uint16_t b_id = bullet_id;
        inc_bullet_id();

        uint16_t range = player_it->second->get_current_weapon().get_range();
        uint16_t damage = player_it->second->get_current_weapon().get_damage();
        std::vector<uint16_t> position = calculate_bullet_starting_position(
                player_it->second->get_position(), desired_position);

        // Crea UNA sola instancia de la bala
        auto bullet_ptr = std::make_shared<Bullet>(b_id, position, range, damage);

        // Setea el target de la bala
        bullet_ptr->set_target_position(desired_position);

        // Agrega la bala a todas las estructuras usando el mismo puntero
        bullets[b_id] = bullet_ptr;
        objects.push_back(bullet_ptr);
        auto cell = get_cell_from_position(bullet_ptr->get_position());
        matrix[cell.first][cell.second].push_back(bullet_ptr);

        // Movimiento inicial de la bala
        auto move_result = _move(*bullet_ptr, bullet_ptr->get_next_position());
        if (move_result.first) {
            bullet_ptr->move(move_result.second);
            return update_object_in_matrix(bullet_ptr, position);
        }
        return false;
    } else {
        std::cout << "No se encontró el jugador con ID: " << player_id << std::endl;
        return false;
    }
}

// Creo que no se necesita más porque se actualiza solo en el movimiento
// void Game::reap() {
//     objects.erase(std::remove_if(objects.begin(), objects.end(),
//                                  [this](const std::shared_ptr<Object>& obj) {
//                                      auto cell = get_cell_from_position(obj->get_position());
//                                      const auto& cell_vec = matrix[cell.first][cell.second];
//                                      // Busca si el objeto está en la celda correspondiente de
//                                      // la matriz
//                                      return std::none_of(
//                                              cell_vec.begin(), cell_vec.end(),
//                                              [&obj](const std::shared_ptr<Object>& o) {
//                                                  return o == obj;
//                                              });
//                                  }),
//                   objects.end());
// }
