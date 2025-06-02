#include "game.h"

#include <algorithm>
#include <iostream>

Game::Game(Config& config):
        matrix(MATRIX_SIZE, std::vector<std::vector<std::shared_ptr<Object>>>(MATRIX_SIZE)),
        config(config),
        weapon_shop(config) {
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
                                                   const std::vector<uint16_t>& new_position) {
    /*
     *    Realiza el movimiento del objeto dado en la dirección dada.
     *    Realiza el movimiento del objeto dado en la dirección dada.
     *    Devuelve un par que indica si el movimiento fue exitoso y la nueva posición.
     */

    std::vector<uint16_t> position = obj.get_position();

    if (new_position == position) {
        return {false, position};  // No se puede mover
    }

    std::cout << "\nIntentando mover " << get_object_type(static_cast<ObjectType>(obj.get_type()))
              << " con ID " << obj.get_id() << " desde (" << position[0] << ", " << position[1]
              << ")"
              << " hacia (" << new_position[0] << ", " << new_position[1] << ")" << std::endl;

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

    // Determinar dirección de movimiento
    int dx = (new_position[0] > max_position[0]) ? 1 : (new_position[0] < max_position[0]) ? -1 : 0;
    int dy = (new_position[1] > max_position[1]) ? 1 : (new_position[1] < max_position[1]) ? -1 : 0;

    uint16_t x = max_position[0];
    uint16_t y = max_position[1];

    while (x != new_position[0] || y != new_position[1]) {
        uint16_t next_x = (x == new_position[0]) ? x : x + dx;
        uint16_t next_y = (y == new_position[1]) ? y : y + dy;
        std::vector<uint16_t> test_position = {next_x, next_y};

        auto collision_result = collides(obj, test_position, ady_objects);
        ObjectType collider_type = static_cast<ObjectType>(collision_result.first);
        if (collider_type != ObjectType::UNKNOWN) {
            ObjectType obj_type = static_cast<ObjectType>(obj.get_type());
            if (collider_type == ObjectType::BULLET && obj_type == ObjectType::PLAYER) {
                // Si un jugador colisiona con una bala, se mueve hasta la posición de la bala
                max_position = test_position;

            } else if (collider_type == ObjectType::PLAYER && obj_type == ObjectType::BULLET) {
                // Si una bala colisiona con un jugador, se mueve hasta la posición del jugador
                max_position = test_position;

            } else if (collider_type == ObjectType::OBSTACLE && obj_type == ObjectType::BULLET) {
                // Si una bala colisiona con un obstáculo, se mueve hasta la posición del
                // obstáculo
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

bool circle_circle_collision(const std::vector<uint16_t>& new_position,
                             const uint16_t& object_radius,
                             const std::vector<uint16_t>& collider_position,
                             const uint16_t& collider_radius) {
    /*
     *    Verifica si hay colisión entre dos círculos dados sus posiciones y radios.
     *    new_position: posición del objeto que se mueve
     *    object_radius: radio del objeto que se mueve
     *    collider_position: posición del objeto con el que se colisiona
     *    collider_radius: radio del objeto con el que se colisiona
     */
    int dx = static_cast<int>(new_position[0]) - static_cast<int>(collider_position[0]);
    int dy = static_cast<int>(new_position[1]) - static_cast<int>(collider_position[1]);
    int distance_squared = dx * dx + dy * dy;

    int radius_sum = object_radius + collider_radius;

    return distance_squared < radius_sum * radius_sum;
}

bool circle_rectangle_collision(const std::vector<uint16_t>& c_center, const uint16_t& c_radius,
                                const std::vector<uint16_t>& r_position, const uint16_t& r_width,
                                const uint16_t& r_height) {
    /*
     *    Verifica si hay colisión entre un círculo y un rectángulo.
     *    c_center: posición del objeto que se mueve
     *    c_radius: radio del objeto que se mueve
     *    r_position: posición del rectángulo con el que se colisiona
     *    r_width: ancho del rectángulo con el que se colisiona
     *    r_height: alto del rectángulo con el que se colisiona
     */

    // Encuentra el punto más cercano al círculo dentro del rectángulo
    int closest_x = std::max(
            static_cast<int>(r_position[0]),
            std::min(static_cast<int>(c_center[0]), static_cast<int>(r_position[0] + r_width)));
    int closest_y = std::max(
            static_cast<int>(r_position[1]),
            std::min(static_cast<int>(c_center[1]), static_cast<int>(r_position[1] + r_height)));

    // Calcula la distancia entre el círculo y este punto más cercano
    int dx = closest_x - static_cast<int>(c_center[0]);
    int dy = closest_y - static_cast<int>(c_center[1]);

    return (dx * dx + dy * dy) < (c_radius * c_radius);
}

std::pair<ObjectType, uint16_t> Game::collides(const Object& object,
                                               const std::vector<uint16_t>& new_position,
                                               const std::set<std::shared_ptr<Object>>& objects) {
    /*
     *    Verifica si hay colisiones con otros objetos en la nueva posición.
     *    Devuelve el tipo de objeto con el que colisiona y su ID.
     */

    uint16_t radius;
    if (object.get_type() == ObjectType::BULLET) {
        radius = BULLET_SIZE / 2;  // Radio de la bala
    } else if (object.get_type() == ObjectType::PLAYER) {
        radius = PLAYER_SIZE / 2;  // Radio del jugador
    } else {
        return {ObjectType::UNKNOWN, 0};  // Tipo desconocido
    }

    for (const auto& obj: objects) {
        if (object == *obj) {
            continue;
        }
        bool overlap = false;

        if (obj->get_type() == ObjectType::BULLET) {
            overlap = circle_circle_collision(new_position, radius, obj->get_position(),
                                              BULLET_SIZE / 2);
        } else if (obj->get_type() == ObjectType::PLAYER) {
            overlap = circle_circle_collision(new_position, radius, obj->get_position(),
                                              PLAYER_SIZE / 2);
        } else if (obj->get_type() == ObjectType::OBSTACLE) {
            overlap = circle_rectangle_collision(new_position, radius, obj->get_position(),
                                                 obj->get_width(), obj->get_height());
        }

        if (overlap) {
            std::cout << "\tColisión detectada entre "
                      << get_object_type(static_cast<ObjectType>(object.get_type()))
                      << " ID: " << object.get_id() << " y "
                      << get_object_type(static_cast<ObjectType>(obj->get_type()))
                      << " ID: " << obj->get_id() << std::endl;

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

    // Si no colisiona con bala, agregar a la nueva celda
    matrix[new_cell.first][new_cell.second].push_back(obj);
    return true;
}

uint16_t Game::get_damage_and_delete_bullet(uint bullet_id) {
    uint16_t damage = 0;
    auto bullet_it = bullets.find(bullet_id);
    if (bullet_it != bullets.end()) {
        damage = bullet_it->second->get_damage();
        // Eliminar la bala de la matriz
        auto bullet_cell = get_cell_from_position(bullet_it->second->get_position());
        uint bullet_id_val = bullet_it->first;
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

        // Eliminar la bala de la lista de balas
        bullets.erase(bullet_it);
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

            // Eliminar al jugador de la matriz
            auto player_cell = get_cell_from_position(player_it->second->get_position());
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

            // Eliminar al jugador de players
            players.erase(player_it);

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
                _move(*(player_it->second), new_position);
        // _move(static_cast<Object>(*(player_it->second)), new_position);
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
    std::map<uint16_t, std::vector<uint16_t>> bullets_to_update;
    std::vector<uint16_t> bullets_to_delete;

    for (const auto& [id, bullet]: bullets) {
        if (bullet->is_alive()) {
            auto move_result = _move(*bullet, bullet->get_next_position());
            if (move_result.first) {
                bullets_to_update[id] = bullet->get_position();
                uint16_t distance = distance_moved(bullet->get_position(), move_result.second);
                bullet->decrement_range(distance);
                bullet->move(move_result.second);

            } else {
                bullets_to_delete.push_back(id);
            }
        } else {
            bullets_to_delete.push_back(id);
        }
    }

    for (const auto& [id, old_position]: bullets_to_update) {
        auto bullet_it = bullets.find(id);
        if (bullet_it != bullets.end()) {
            auto bullet = bullet_it->second;
            update_object_in_matrix(bullet, old_position);
        }
    }

    for (auto id: bullets_to_delete) {
        get_damage_and_delete_bullet(id);
    }
}

bool Game::shoot(const std::vector<uint16_t>& desired_position, const uint16_t player_id) {
    auto player_it = players.find(player_id);
    if (player_it != players.end()) {
        uint16_t b_id = bullet_id;
        inc_bullet_id();

        uint16_t range = player_it->second->get_current_weapon().get_range();
        uint16_t damage = player_it->second->get_current_weapon().get_damage();
        std::vector<uint16_t> player_position = player_it->second->get_position();

        Bullet bullet(b_id, player_position, range, damage, desired_position);

        auto bullet_ptr = std::make_shared<Bullet>(bullet);
        bullets[b_id] = bullet_ptr;
        objects.push_back(bullet_ptr);
        auto cell = get_cell_from_position(bullet_ptr->get_position());
        matrix[cell.first][cell.second].push_back(bullet_ptr);

        auto move_result = _move(*bullet_ptr, bullet_ptr->get_next_position());
        if (move_result.first) {
            std::vector<uint16_t> old_position = bullet_ptr->get_position();
            uint16_t distance = distance_moved(bullet_ptr->get_position(), move_result.second);
            bullet_ptr->decrement_range(distance);
            bullet_ptr->move(move_result.second);
            return update_object_in_matrix(bullet_ptr, old_position);
        }
        return false;
    } else {
        std::cout << "No se encontró el jugador con ID: " << player_id << std::endl;
        return false;
    }
}

std::vector<std::shared_ptr<Object>>& Game::get_objects() {
    // reap();
    update_bullets();
    return objects;
}

Player Game::get_player_with_random_position(PlayerType player_type, uint16_t id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0 + PLAYER_SIZE, MATRIX_SIZE - PLAYER_SIZE);

    while (true) {
        std::vector<uint16_t> pos = {dist(gen), dist(gen)};
        auto cell = get_cell_from_position(pos);
        auto ady = get_adyacent_objects(cell);
        Player temp_player(id, pos, player_type, config.get_player_health(),
                           config.get_player_money(), weapon_shop, false);
        auto collision = collides(temp_player, pos, ady);
        if (collision.first == ObjectType::UNKNOWN) {
            return temp_player;
        }
    }
}

void Game::add_player(PlayerType player_type, uint16_t id) {
    std::shared_ptr<Player> player =
            std::make_shared<Player>(get_player_with_random_position(player_type, id));

    // Agregar el jugador a players
    players.insert({id, player});

    // Agregar el jugador a objects
    objects.push_back(player);

    // Agregar el jugador a la matriz
    auto cell = get_cell_from_position(player->get_position());
    matrix[cell.first][cell.second].push_back(player);
}

bool Game::is_over() {
    /*
     * Una ronda termina cuando:
     *     - plantan una bomba y explota
     *     - desactivan bomba
     *     - todos los jugadores de un bando eliminados
     */
    // TODO: Por ahora, el juego termina si no hay jugadores de ambos bandos
    return !(std::any_of(players.begin(), players.end(),
                         [](const auto& par) {
                             return par.second &&
                                    par.second->get_player_type() == PlayerType::TERRORIST;
                         }) &&
             std::any_of(players.begin(), players.end(), [](const auto& par) {
                 return par.second && par.second->get_player_type() == PlayerType::COUNTERTERRORIST;
             }));
}

bool Game::is_ready_to_start() {
    // Empieza el juego si hay al menos un TERRORIST y un COUNTERTERRORIST
    return std::any_of(players.begin(), players.end(),
                       [](const auto& p) {
                           return p.second && p.second->get_player_type() == PlayerType::TERRORIST;
                       }) &&
           std::any_of(players.begin(), players.end(), [](const auto& p) {
               return p.second && p.second->get_player_type() == PlayerType::COUNTERTERRORIST;
           });
}
