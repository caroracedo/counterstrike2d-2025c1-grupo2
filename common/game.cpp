#include "game.h"

Game::Game(Config& config, Map& map):
        matrix(MATRIX_SIZE, std::vector<std::vector<std::shared_ptr<Object>>>(MATRIX_SIZE)),
        config(config),
        map(map),
        weapon_shop(config) {
    initialize_objects();
}

bool Game::is_ready_to_start() {
    /*
        Devuelve si hay jugadores suficientes en cada equipo.
    */
    return _is_ready_to_start ||
           (std::count_if(players.begin(), players.end(),
                          [](const auto& p) {
                              return p.second &&
                                     p.second->get_player_type() == PlayerType::TERRORIST;
                          }) == config.get_rounds_terrorist() &&
            std::count_if(players.begin(), players.end(), [](const auto& p) {
                return p.second && p.second->get_player_type() == PlayerType::COUNTERTERRORIST;
            }) == config.get_rounds_counterterrorist());
}

void Game::set_ready_to_start() {
    /*
        Permite al jugador dar comienzo a la partida si hay por lo menos un jugador en cada equipo
    */
    _is_ready_to_start =
            (std::count_if(players.begin(), players.end(),
                           [](const auto& p) {
                               return p.second &&
                                      p.second->get_player_type() == PlayerType::TERRORIST;
                           }) >= 1 &&
             std::count_if(players.begin(), players.end(), [](const auto& p) {
                 return p.second && p.second->get_player_type() == PlayerType::COUNTERTERRORIST;
             }) >= 1);
}

std::vector<std::shared_ptr<Object>>& Game::get_objects() {
    /*
        Devuelve el vector de objetos del juego actualizado.
    */
    update_bullets();
    update_bomb_countdown();
    return objects;
}

void Game::add_player(PlayerType player_type, uint16_t id) {
    /*
        Agrega un jugador al juego con el tipo y ID especificados.
    */
    uint8_t max_player_amount =
            config.get_rounds_terrorist() + config.get_rounds_counterterrorist();
    if (players.size() >= max_player_amount) {
        std::cerr << "Maximum amount of players reached. Limit: " << max_player_amount << std::endl;
        return;
    }

    std::vector<uint16_t> position = get_random_player_position(player_type, id);

    std::shared_ptr<Player> player =
            std::make_shared<Player>(id, position, player_type, config.get_player_health(),
                                     config.get_player_money(), weapon_shop);

    // Agregar el jugador a players
    players.insert({id, player});

    // Agregar el jugador a objects
    objects.push_back(player);

    // Agregar el jugador a la matriz
    auto cell = get_cell_from_position(player->get_position());
    matrix[cell.first][cell.second].push_back(player);
}

bool Game::move(Direction direction, const uint16_t& id) {
    /*
        Mueve al jugador con el ID especificado en la dirección dada.
     */

    // Buscar el jugador con el ID especificado
    auto player_it = players.find(id);
    if (player_it != players.end()) {
        std::vector<uint16_t> old_position = player_it->second->get_position();
        std::vector<uint16_t> new_position = player_it->second->get_next_position(direction);
        std::pair<bool, std::vector<uint16_t>> move_result =
                _move(*(player_it->second), new_position);
        if (move_result.first) {
            // Actualizar la posición del jugador
            player_it->second->move(move_result.second);

            // Actualizar el objeto en la matriz y retornar si sigue vivo
            return update_object_in_matrix(std::static_pointer_cast<Object>(player_it->second),
                                           old_position) ||
                   old_position != move_result.second;
        }
    } else {
        std::cerr << "Player with ID " << id << " not found." << std::endl;
        return false;
    }
    return false;
}

void Game::rotate_player(float angle, uint16_t id) {
    /*
        Rota al jugador con el ID especificado en el ángulo dado.
    */
    auto player_it = players.find(id);
    if (player_it != players.end()) {
        player_it->second->rotate(angle);
    } else {
        std::cerr << "[GAME] Player with ID " << id << " not found.\n";
    }
}

bool Game::shoot(const std::vector<uint16_t>& desired_position, const uint16_t player_id) {
    /*
        Dispara desde la posición del jugador hacia la posición deseada, con el arma actual del
       jugador.
    */
    auto player_it = players.find(player_id);
    if (player_it != players.end()) {

        if (!player_it->second->shoot()) {
            return false;  // El jugador no puede disparar con el arma actual
        }
        std::vector<uint16_t> player_position = player_it->second->get_position();

        WeaponDTO weapon_dto = player_it->second->get_current_weapon();

        if (weapon_dto.model == WeaponModel::AWP || weapon_dto.model == WeaponModel::GLOCK) {
            create_bullet(player_position, player_id, weapon_dto, desired_position);
            return true;
        } else if (weapon_dto.model == WeaponModel::M3) {
            return shoot_m3(player_position, player_id, weapon_dto, desired_position);
        } else if (weapon_dto.model == WeaponModel::AK47) {
            return shoot_ak47(player_position, player_id, weapon_dto, desired_position);
        } else if (weapon_dto.model == WeaponModel::KNIFE) {
            employ_knife(player_position, player_id, weapon_dto, desired_position);
            return true;
        }

        return false;

    } else {
        std::cerr << "Player with ID " << player_id << " not found." << std::endl;
        return false;
    }
}

void Game::pick_up_weapon(uint16_t id) {
    /*
        Permite al jugador con el ID especificado recoger un arma o una bomba si está cerca.
    */
    auto players_it = players.find(id);
    if (players_it == players.end()) {
        return;
    }
    std::vector<uint16_t> player_position = players_it->second->get_position();
    auto cell = get_cell_from_position(player_position);
    auto adyacent_objects = get_adyacent_objects(cell);
    auto player_ptr = players_it->second;

    for (const auto& obj: adyacent_objects) {
        if (obj->get_type() == ObjectType::WEAPON) {
            bool is_near_weapon = circle_rectangle_collision(player_position, PLAYER_RADIUS + 1,
                                                             obj->get_position(), obj->get_width(),
                                                             obj->get_height());
            if (is_near_weapon) {
                WeaponDTO new_weapon_dto = delete_weapon(obj->get_id());
                WeaponDTO old_weapon_dto = player_ptr->pick_up_weapon(new_weapon_dto);
                if (old_weapon_dto.model != WeaponModel::UNKNOWN) {
                    create_weapon(old_weapon_dto, player_position);
                }
                return;
            }
        } else if (obj->get_type() == ObjectType::BOMB &&
                   distance_between(player_position, obj->get_position()) <=
                           PLAYER_RADIUS + BOMB_RADIUS + 1) {
            WeaponDTO bomb_dto;
            bomb_dto.model = WeaponModel::BOMB;
            player_ptr->pick_up_weapon(bomb_dto);
            if (player_ptr->can_plant_bomb()) {
                delete_bomb();
                return;
            }
        }
    }
}

bool Game::interact_with_bomb(const uint16_t& player_id) {
    /*
        Permite interactuar con la bomba, ya sea para activarla si el jugador es terrorista y tiene
        la bomba, o desactivarla si el jugador es counter-terrorista. Devuelve true si la
       interacción es exitosa, false en caso contrario.
    */
    auto player_it = players.find(player_id);
    if (player_it != players.end()) {
        std::vector<uint16_t> player_position = player_it->second->get_position();
        PlayerType player_type = player_it->second->get_player_type();
        if (player_type == PlayerType::COUNTERTERRORIST) {
            return deactivate_bomb(player_position);
        } else if (player_type == PlayerType::TERRORIST && player_it->second->can_plant_bomb()) {
            if (is_in_bomb_zone(player_position)) {
                player_it->second->plant_bomb();
                return plant_bomb(player_position);
            }
        }
    } else {
        std::cerr << "Player with ID " << player_id << " not found." << std::endl;
    }
    return false;
}

bool Game::is_over() {
    /*
        Una ronda termina cuando:
         - plantan una bomba y explota
         - desactivan bomba
         - todos los jugadores de un bando eliminados
    */

    bool terrorists_alive = std::any_of(players.begin(), players.end(), [](const auto& par) {
        return par.second && par.second->get_player_type() == PlayerType::TERRORIST;
    });
    bool cts_alive = std::any_of(players.begin(), players.end(), [](const auto& par) {
        return par.second && par.second->get_player_type() == PlayerType::COUNTERTERRORIST;
    });

    bool finished = !(terrorists_alive && cts_alive) || exploded || deactivated;
    if (finished) {
        // Determinar qué equipo es team_a y team_b según el round
        bool team_a_is_terrorist = (round_number <= 5);

        PlayerType winner = PlayerType::UNKNOWN;
        if ((!terrorists_alive && cts_alive) || deactivated) {
            winner = PlayerType::COUNTERTERRORIST;
        } else if ((!cts_alive && terrorists_alive) || exploded) {
            winner = PlayerType::TERRORIST;
        }

        stats.last_winner = winner;

        // Sumar ronda ganada al equipo correcto
        if (winner != PlayerType::UNKNOWN) {
            if ((winner == PlayerType::TERRORIST && team_a_is_terrorist) ||
                (winner == PlayerType::COUNTERTERRORIST && !team_a_is_terrorist)) {
                stats.team_a_wins++;
            } else {
                stats.team_b_wins++;
            }
        }
    }
    return finished;
}

void Game::start_round_game_phase() {
    /*
        Inicia la fase de juego de la ronda, eligiendo al jugador que tendrá la bomba, y se
       actualiza el número de ronda
    */
    set_bomb_player();
    ++round_number;
}

void Game::end_round_game_phase() {
    /*
        Reinicia la fase de juego de la ronda:
            - Se elimina la bomba si existe -> exploded=false y deactivated=false
            - Se reestablecen a los jugadores muertos
            - Se reubican a todos los jugadores en posiciones aleatorias
            - Se eliminan todas las balas que quedaron vivas y se reinicia el contador de balas
    */
    if (bomb) {
        delete_bomb();
        exploded = false;
    } else {
        deactivated = false;
    }
    for (auto& [id, player]: dead_players) {
        if (player) {
            // Reestablece la salud del jugador muerto
            player->cure(config.get_player_health());

            // Reagrega al jugador a la lista de jugadores
            players[id] = player;

            // Reagrega al jugador a la lista de objetos
            objects.push_back(player);
        }
    }
    // Elimina los jugadores muertos de la lista de jugadores muertos
    dead_players.clear();

    // Reubica los jugadores en posiciones aleatorias
    for (auto& [id, player]: players) {
        if (player) {
            // Elimina al jugador de la celda actual de la matriz
            remove_from_matrix(ObjectType::PLAYER, id, player->get_position());

            // Actualiza la posición del jugador
            std::vector<uint16_t> new_position =
                    get_random_player_position(player->get_player_type(), id);
            player->move(new_position);

            // Actualiza la posición del jugador en la matriz
            auto cell = get_cell_from_position(new_position);
            matrix[cell.first][cell.second].push_back(player);
        }
    }

    // Obtener los ids de las balas
    std::vector<uint16_t> bullet_ids;
    for (const auto& [id, bullet]: bullets) {
        if (bullet) {
            bullet_ids.push_back(id);
        }
    }
    // Elimina las balas del juego
    for (const auto& id: bullet_ids) {
        delete_bullet(id);
    }

    // Reinicia el contador de balas
    bullet_id = 1;
}

void Game::switch_player_types() {
    /*
        Cambia el tipo de todos los jugadores en el juego, de terrorista a counter-terrorista y
       viceversa.
    */
    for (auto& [id, player]: players) {
        if (player) {
            player->switch_player_type();
        }
    }
}

bool Game::shop_weapon(WeaponModel weapon, uint16_t id) {
    /*
        Permite a los jugadores comprar un arma en la fase inicial
    */
    auto player_it = players.find(id);
    if (player_it != players.end() && player_it->second->buy_weapon(weapon)) {
        return true;
    }
    return false;
}

bool Game::shop_ammo(uint ammo, WeaponType weapon, uint16_t id) {
    /*
        Permite a los jugadores comprar munición en la fase inicial
    */
    auto player_it = players.find(id);
    if (player_it != players.end()) {
        return player_it->second->buy_ammo(weapon, ammo);
    }
    return false;
}

bool Game::change_weapon(uint16_t id) {
    /*
        Permite al jugador con el ID especificado cambiar de arma.
        Devuelve true si el cambio fue exitoso, false si no se encontró el jugador.
    */
    auto player_it = players.find(id);
    if (player_it != players.end()) {
        player_it->second->change_weapon();
        return true;
    }
    return false;
}

Stats Game::get_stats() const { return stats; }

void Game::quit(uint16_t id) {
    /*
        Elimina al jugador de la partida
    */
    auto player_it = players.find(id);
    if (player_it != players.end()) {
        // Eliminar al jugador de la matriz
        remove_from_matrix(ObjectType::PLAYER, id, player_it->second->get_position());

        // Eliminar al jugador de los objetos
        remove_from_objects(ObjectType::PLAYER, id);

        // Eliminar al jugador de players
        players.erase(player_it);
    }
}

/*******************************************************************************************
 *****************************************POSICIONES Y CELDAS*******************************
 ******************************************************************************************/

std::pair<uint16_t, uint16_t> Game::get_cell_from_position(const std::vector<uint16_t>& position) {
    /*
        Obtiene la celda correspondiente a una posición dada.
    */
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
        Obtiene las celdas adyacentes a la celda dada dentro de un radio especificado.
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

uint16_t Game::distance_between(const std::vector<uint16_t>& position1,
                                const std::vector<uint16_t>& position2) {
    /*
        Calcula la distancia entre dos posiciones dadas.
    */
    int x1 = static_cast<int>(position1[0]);
    int y1 = static_cast<int>(position1[1]);
    int x2 = static_cast<int>(position2[0]);
    int y2 = static_cast<int>(position2[1]);
    return static_cast<uint16_t>(std::sqrt(std::pow((x2 - x1), 2) + std::pow(y2 - y1, 2)));
}

std::set<std::shared_ptr<Object>> Game::get_adyacent_objects(
        const std::pair<uint16_t, uint16_t>& cell) {
    /*
        Obtiene los objetos en las celdas adyacentes a la celda dada.
    */

    // Vector con todas las celdas adyacentes y las adyacentes a estas
    std::vector<std::pair<uint16_t, uint16_t>> adyacent_cells = get_cells_within_radius(cell);

    std::set<std::shared_ptr<Object>> objects_set;
    for (const auto& adyacent_cell: adyacent_cells) {
        const auto& cell_objects = matrix[adyacent_cell.first][adyacent_cell.second];
        for (const auto& obj: cell_objects) {
            objects_set.insert(obj);
        }
    }

    return objects_set;
}

/*******************************************************************************************
 ***************************************** MOVIMIENTO **************************************
 ******************************************************************************************/

std::pair<bool, std::vector<uint16_t>> Game::_move(const Object& obj,
                                                   const std::vector<uint16_t>& new_position) {
    /*
        Realiza el movimiento del objeto dado en la dirección dada.
        Devuelve un par que indica si el movimiento fue exitoso y la nueva posición.
    */

    std::vector<uint16_t> position = obj.get_position();

    if (new_position == position) {
        return {false, position};  // No se puede mover
    }

    std::vector<uint16_t> max_position = get_max_position(obj, new_position);

    return {max_position != position, max_position};
}

std::vector<uint16_t> Game::get_max_position(const Object& obj,
                                             const std::vector<uint16_t>& new_position) {
    /*
        Determina la posición máxima a la que se puede mover el objeto sin colisionar.
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

std::pair<ObjectType, uint16_t> Game::collides(const Object& object,
                                               const std::vector<uint16_t>& new_position,
                                               const std::set<std::shared_ptr<Object>>& objects) {
    /*
        Verifica si hay colisiones con otros objetos en la nueva posición.
        Devuelve el tipo de objeto con el que colisiona y su ID.
    */

    if (object.get_type() == ObjectType::UNKNOWN) {
        return {ObjectType::UNKNOWN, 0};
    }

    uint16_t radius;
    if (object.get_type() == ObjectType::BULLET) {
        radius = BULLET_RADIUS;
    } else if (object.get_type() == ObjectType::PLAYER) {
        radius = PLAYER_RADIUS;
    }

    for (const auto& obj: objects) {
        if (object == *obj) {
            continue;
        }
        bool overlap = false;

        if (obj->get_type() == ObjectType::BULLET) {
            overlap = circle_circle_collision(new_position, radius, obj->get_position(),
                                              BULLET_RADIUS) &&
                      object.get_type() != ObjectType::BULLET;
        } else if (obj->get_type() == ObjectType::PLAYER) {
            overlap = circle_circle_collision(new_position, radius, obj->get_position(),
                                              PLAYER_RADIUS);
        } else if (obj->get_type() == ObjectType::OBSTACLE) {
            overlap = circle_rectangle_collision(new_position, radius, obj->get_position(),
                                                 obj->get_width(), obj->get_height());
        }

        if (overlap) {
            ObjectType type = static_cast<ObjectType>(obj->get_type());
            if (type != ObjectType::UNKNOWN) {
                return {type, obj->get_id()};
            }
            return {ObjectType::UNKNOWN, 0};
        }
    }
    return {ObjectType::UNKNOWN, 0};  // No hay colisión
}

bool Game::circle_circle_collision(const std::vector<uint16_t>& new_position,
                                   const uint16_t& object_radius,
                                   const std::vector<uint16_t>& collider_position,
                                   const uint16_t& collider_radius) {
    /*
        Verifica si hay colisión entre dos círculos dados sus posiciones y radios.
        new_position: posición del objeto que se mueve
        object_radius: radio del objeto que se mueve
        collider_position: posición del objeto con el que se colisiona
        collider_radius: radio del objeto con el que se colisiona
    */
    int dx = static_cast<int>(new_position[0]) - static_cast<int>(collider_position[0]);
    int dy = static_cast<int>(new_position[1]) - static_cast<int>(collider_position[1]);
    int distance_squared = dx * dx + dy * dy;

    int radius_sum = object_radius + collider_radius;

    return distance_squared < radius_sum * radius_sum;
}

bool Game::circle_rectangle_collision(const std::vector<uint16_t>& c_center,
                                      const uint16_t& c_radius,
                                      const std::vector<uint16_t>& r_position,
                                      const uint16_t& r_width, const uint16_t& r_height) const {
    /*
        Verifica si hay colisión entre un círculo y un rectángulo.
        c_center: posición del objeto que se mueve
        c_radius: radio del objeto que se mueve
        r_position: posición del rectángulo con el que se colisiona
        r_width: ancho del rectángulo con el que se colisiona
        r_height: alto del rectángulo con el que se colisiona
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


bool Game::update_object_in_matrix(const std::shared_ptr<Object>& obj,
                                   const std::vector<uint16_t>& old_position) {
    /*
        Actualiza la posición del objeto en la matriz.
        Elimina el objeto de la celda anterior y lo agrega a la nueva celda si sigue vivo.
        Devuelve true si el objeto sigue vivo, false si no.
    */

    // Quitar de la celda anterior
    remove_from_matrix(obj->get_type(), obj->get_id(), old_position);

    // Chequear colisión en la nueva celda
    auto new_cell = get_cell_from_position(obj->get_position());
    auto collision_result = collides(*obj, obj->get_position(), get_adyacent_objects(new_cell));
    ObjectType collider_type = static_cast<ObjectType>(collision_result.first);
    uint16_t collider_id = collision_result.second;

    if (collider_type == ObjectType::BULLET &&
        static_cast<ObjectType>(obj->get_type()) == ObjectType::PLAYER) {
        // Jugador colisiona con una bala
        uint16_t shooter_id = bullets.find(collider_id)->second->get_player_id();
        bool player_is_alive =
                handle_bullet_player_collision(collider_id, shooter_id, obj->get_id());
        if (!player_is_alive) {
            return false;  // El jugador ha muerto, no se mueve
        }

    } else if (collider_type == ObjectType::PLAYER &&
               static_cast<ObjectType>(obj->get_type()) == ObjectType::BULLET) {
        // Bala colisiona con un jugador
        uint16_t shooter_id = bullets.find(obj->get_id())->second->get_player_id();
        handle_bullet_player_collision(obj->get_id(), shooter_id, collider_id);
        return false;  // La bala no se mueve si colisiona con un jugador

    } else if (collider_type == ObjectType::OBSTACLE &&
               static_cast<ObjectType>(obj->get_type()) == ObjectType::BULLET) {
        // Bala colisiona con un obstáculo
        delete_bullet(obj->get_id());
        return false;  // La bala no se mueve si colisiona con un obstáculo
    }

    matrix[new_cell.first][new_cell.second].push_back(obj);
    return true;
}

bool Game::handle_bullet_player_collision(const uint16_t& bullet_id, const uint16_t& shooter_id,
                                          const uint16_t& player_id) {
    /*
        Maneja la colisión entre una bala y un jugador.
        Inflige daño al jugador y elimina la bala.
        Devuelve si el jugador sigue vivo.
    */
    uint16_t damage = get_damage_and_delete_bullet(bullet_id);
    bool player_is_alive = damage_player(player_id, damage);
    if (!player_is_alive) {
        // El jugador murió, se le da dinero al jugador que disparó
        players.find(shooter_id)->second->add_money(KILL_REWARD);

        stats.kills[shooter_id]++;
        stats.deaths[player_id]++;
        stats.money[shooter_id] += KILL_REWARD;
    }
    return player_is_alive;
}

/*******************************************************************************************
 *********************************ACTUALIZACIÓN DE JUGADORES Y BALAS************************
 ******************************************************************************************/

bool Game::damage_player(uint16_t id, uint16_t damage) {
    /*
        Inflige daño al jugador con el ID especificado y devuelve si sigue vivo.
        Si el jugador muere, lo elimina de la lista de jugadores, de la matriz y de los objetos.
    */
    auto player_it = players.find(id);
    if (player_it != players.end()) {
        player_it->second->take_damage(damage);

        if (!player_it->second->is_alive()) {
            drop_weapons(player_it->second->get_id());
            dead_players[id] = player_it->second;
            remove_from_matrix(ObjectType::PLAYER, id, player_it->second->get_position());
            remove_from_objects(ObjectType::PLAYER, id);
            players.erase(player_it);

            return false;  // El jugador ha muerto
        }

        return true;  // El jugador sigue vivo
    }
    std::cerr << "\tPlayer " << id << " not found." << std::endl;
    return false;
}

void Game::create_bullet(const std::vector<uint16_t>& player_position, const u_int16_t player_id,
                         const WeaponDTO& weapon_dto,
                         const std::vector<uint16_t>& desired_position) {
    /*
        Crea una bala y la agrega a la matriz, al vector de objetos y al mapa de balas.
    */
    uint16_t id = bullet_id;
    inc_bullet_id();

    Bullet bullet(id, player_id, player_position, weapon_dto.range, weapon_dto.min_damage,
                  weapon_dto.max_damage, weapon_dto.precision, desired_position);
    auto bullet_ptr = std::make_shared<Bullet>(bullet);

    // Agregar la bala al mapa de balas
    bullets[id] = bullet_ptr;

    // Agregar la bala a la lista de objetos
    objects.push_back(bullet_ptr);

    // Agregar la bala a la matriz
    auto cell = get_cell_from_position(bullet_ptr->get_position());
    matrix[cell.first][cell.second].push_back(bullet_ptr);
}

uint16_t Game::get_damage_and_delete_bullet(const uint16_t& bullet_id) {
    /*
        Devuelve el daño que inflige la bala y la elimina
    */
    auto bullet_it = bullets.find(bullet_id);
    if (bullet_it != bullets.end()) {
        uint16_t damage = bullet_it->second->get_damage();
        delete_bullet(bullet_id);
        return damage;
    }
    return 0;
}

void Game::delete_bullet(const uint16_t& bullet_id) {
    /*
        Elimina la bala con el ID especificado de la matriz, de los objetos y del mapa de balas.
        Si no se encuentra la bala, no hace nada.
    */
    auto bullet_it = bullets.find(bullet_id);
    if (bullet_it != bullets.end()) {
        // Eliminar la bala de la matriz
        remove_from_matrix(ObjectType::BULLET, bullet_id, bullet_it->second->get_position());

        // Eliminar la bala de los objetos
        remove_from_objects(ObjectType::BULLET, bullet_id);

        // Eliminar la bala de la lista de balas
        bullets.erase(bullet_it);
    }
}

void Game::inc_bullet_id() {
    /*
        Incrementa el ID de la bala para que no se repita.
    */
    bullet_id++;
    if (bullet_id == 0) {
        bullet_id = 1;
    }
}

void Game::update_bullets() {
    /*
        Actualiza las balas en la matriz, eliminando las que ya no están activas.
    */
    update_ak47_bursts();

    std::map<uint16_t, std::vector<uint16_t>> bullets_to_update;
    std::vector<uint16_t> bullets_to_delete;

    for (const auto& [id, bullet]: bullets) {
        if (bullet->is_alive()) {
            auto move_result = _move(*bullet, bullet->get_next_position());
            if (move_result.first) {
                bullets_to_update[id] = bullet->get_position();
                uint16_t distance = distance_between(bullet->get_position(), move_result.second);
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
        delete_bullet(id);
    }
}

bool Game::shoot_m3(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                    const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position) {
    /*
        Dispara 3 balas en cono
    */
    double dx = static_cast<double>(desired_position[0] - player_position[0]);
    double dy = static_cast<double>(desired_position[1] - player_position[1]);
    double magnitude = std::sqrt(dx * dx + dy * dy);

    if (magnitude == 0)
        return false;

    double ux = dx / magnitude;
    double uy = dy / magnitude;

    constexpr double rad_30 = M_PI / 6.0;  // 30 grados en radianes
    double cos_30 = std::cos(rad_30);
    double sin_30 = std::sin(rad_30);

    uint16_t dist = PLAYER_RADIUS + BULLET_RADIUS;

    uint16_t x1 = static_cast<uint16_t>(player_position[0] + dist * (ux * cos_30 - uy * sin_30));
    uint16_t y1 = static_cast<uint16_t>(player_position[1] + dist * (ux * sin_30 + uy * cos_30));

    uint16_t x2 = static_cast<uint16_t>(player_position[0] + dist * (ux * cos_30 + uy * sin_30));
    uint16_t y2 = static_cast<uint16_t>(player_position[1] + dist * (-ux * sin_30 + uy * cos_30));

    create_bullet(player_position, player_id, weapon_dto, desired_position);  // Disparo central
    create_bullet(player_position, player_id, weapon_dto, {x1, y1});          // Disparo izquierdo
    create_bullet(player_position, player_id, weapon_dto, {x2, y2});          // Disparo derecho
    return true;
}

bool Game::shoot_ak47(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                      const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position) {
    /*
        Dispara 3 balas en ráfaga (una atrás de la otra)
    */
    Ak47Burst burst{player_position, player_id, weapon_dto, desired_position, 3, 0.0f};
    ak47_bursts.push_back(burst);
    return true;
}

void Game::update_ak47_bursts() {
    /*
        Actualiza las ráfagas de AK-47, eliminando las que ya no están activas y disparando las que
       aún tienen disparos restantes.
    */
    float delta_seconds = 1.0f / 30.0f;
    for (auto it = ak47_bursts.begin(); it != ak47_bursts.end();) {
        it->time_until_next_shot -= delta_seconds;
        if (it->time_until_next_shot <= 0.0f && it->shots_left > 0) {
            create_bullet(it->player_position, it->player_id, it->weapon_dto, it->desired_position);
            it->shots_left--;
            it->time_until_next_shot = 0.4f;
        }
        if (it->shots_left == 0) {
            it = ak47_bursts.erase(it);
        } else {
            ++it;
        }
    }
}

void Game::employ_knife(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                        const WeaponDTO& weapon_dto,
                        const std::vector<uint16_t>& desired_position) {
    /*
        Emplea el cuchillo para atacar a un jugador en la posición deseada.
    */
    Knife knife(player_position, player_id, weapon_dto.range, weapon_dto.min_damage,
                weapon_dto.max_damage, desired_position);

    auto cell = get_cell_from_position(player_position);
    auto adyacent_cells = get_cells_within_radius(cell, 1);

    for (const auto& ady_cell: adyacent_cells) {
        const auto& cell_objects = matrix[ady_cell.first][ady_cell.second];
        for (const auto& obj: cell_objects) {
            if (!obj || obj->get_type() != ObjectType::PLAYER)
                continue;
            const auto& target_pos = obj->get_position();
            if (target_pos == player_position)
                continue;

            if (knife.hits(target_pos)) {
                uint16_t damage = knife.get_damage();
                damage_player(obj->get_id(), damage);
                return;  // Solo al primer jugador encontrado
            }
        }
    }
}

void Game::create_weapon(const WeaponDTO& weapon_dto, const std::vector<uint16_t>& position) {
    /*
        Crea un arma a partir de un WeaponDTO y la agrega a la matriz, al vector de objetos y al
        mapa de armas. La posición del arma se establece en la posición dada.
    */
    Weapon weapon(weapon_dto);
    weapon.move(position);

    auto weapon_ptr = std::make_shared<Weapon>(weapon);

    // Agrega el arma al vector de armas
    weapons[weapon_dto.id] = weapon_ptr;

    // Agrega el arma en la matriz
    auto cell = get_cell_from_position(position);
    matrix[cell.first][cell.second].push_back(weapon_ptr);

    // Agrega el arma al vector de objetos
    objects.push_back(weapon_ptr);
}

WeaponDTO Game::delete_weapon(const uint16_t weapon_id) {
    /*
        Elimina el arma con el ID especificado y devuelve su WeaponDTO.
    */
    auto it = weapons.find(weapon_id);
    if (it != weapons.end()) {
        auto weapon_ptr = it->second;
        WeaponDTO weapon_dto = weapon_ptr->get_weapon_dto();

        remove_from_matrix(ObjectType::WEAPON, weapon_ptr->get_id(), weapon_ptr->get_position());
        remove_from_objects(ObjectType::WEAPON, weapon_ptr->get_id());

        weapons.erase(it);
        return weapon_dto;
    }
    return WeaponDTO();
}

void Game::drop_weapons(uint16_t id) {
    /*
        Suelta el arma principal y la bomba (si la tiene) del jugador con el ID especificado
    */
    auto players_it = players.find(id);
    if (players_it == players.end()) {
        std::cerr << "[GAME] Player with ID " << id << " not found.\n";
        return;
    }
    auto result = players_it->second->drop_weapons();
    std::vector<uint16_t> position = players_it->second->get_position();
    std::vector<uint16_t> weapon_position = {static_cast<uint16_t>(position[0] - PLAYER_RADIUS),
                                             static_cast<uint16_t>(position[1] - PLAYER_RADIUS)};
    create_weapon(result.first, weapon_position);
    if (result.second) {
        std::vector<uint16_t> bomb_position = {static_cast<uint16_t>(position[0] + 8),
                                               static_cast<uint16_t>(position[1] + 8)};
        create_bomb(bomb_position);
    }
    return;
}

/*******************************************************************************************
 *******************************************INICIALIZACION**********************************
 ******************************************************************************************/

void Game::initialize_objects() {
    /*
        Inicializa los obstáculos y zonas de bomba del juego a partir de la configuración.
    */
    for (const auto& obs_cfg: map.get_obstacles()) {
        auto obstacle = std::make_shared<Obstacle>(std::vector<uint16_t>{obs_cfg.x, obs_cfg.y},
                                                   obs_cfg.width, obs_cfg.height, obs_cfg.type);
        objects.push_back(obstacle);

        auto cell = get_cell_from_position(obstacle->get_position());
        matrix[cell.first][cell.second].push_back(obstacle);
    }
    for (const auto& bomb_zone_cfg: map.get_bomb_zones()) {
        auto bomb_zone =
                std::make_shared<BombZone>(std::vector<uint16_t>{bomb_zone_cfg.x, bomb_zone_cfg.y},
                                           bomb_zone_cfg.width, bomb_zone_cfg.height);
        objects.push_back(bomb_zone);

        auto cell = get_cell_from_position(bomb_zone->get_position());
        matrix[cell.first][cell.second].push_back(bomb_zone);
    }
    for (const auto& weapon_object_cfg: map.get_weapon_objects()) {
        create_weapon(weapon_shop.give_weapon(weapon_object_cfg.type).get_weapon_dto(),
                      {weapon_object_cfg.x, weapon_object_cfg.y});
    }
}

std::vector<uint16_t> Game::get_random_player_position(PlayerType player_type, uint16_t id) {
    /*
        Devuelve una posición aleatoria válida para un jugador, dentro de la zona de su equipo.
        Si no hay zona definida, busca en todo el mapa, siempre chequeando colisiones.
    */
    const auto& zones = map.get_team_zones();
    auto it = std::find_if(zones.begin(), zones.end(),
                           [player_type](const auto& z) { return z.team == player_type; });

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist_x, dist_y;

    if (it == zones.end() || it->width < PLAYER_RADIUS || it->height < PLAYER_RADIUS) {
        // Fallback: todo el mapa
        dist_x = std::uniform_int_distribution<uint16_t>(PLAYER_RADIUS,
                                                         MATRIX_SIZE * CELL_SIZE - PLAYER_RADIUS);
        dist_y = std::uniform_int_distribution<uint16_t>(PLAYER_RADIUS,
                                                         MATRIX_SIZE * CELL_SIZE - PLAYER_RADIUS);
    } else {
        uint16_t ZONE_SIZE = 500;
        // Zona de equipo
        uint16_t center_x = it->x + it->width / 2;
        uint16_t center_y = it->y + it->height / 2;

        int32_t temp_x0 = static_cast<int32_t>(center_x) - ZONE_SIZE / 2;
        int32_t temp_y0 = static_cast<int32_t>(center_y) - ZONE_SIZE / 2;

        uint16_t x0 = static_cast<uint16_t>(
                std::clamp(temp_x0, 0, static_cast<int32_t>(MATRIX_SIZE * CELL_SIZE)));
        uint16_t y0 = static_cast<uint16_t>(
                std::clamp(temp_y0, 0, static_cast<int32_t>(MATRIX_SIZE * CELL_SIZE)));

        uint16_t x1 = std::clamp(temp_x0 + ZONE_SIZE, temp_x0 + ZONE_SIZE, MATRIX_SIZE * CELL_SIZE);
        uint16_t y1 = std::clamp(temp_y0 + ZONE_SIZE, temp_y0 + ZONE_SIZE, MATRIX_SIZE * CELL_SIZE);

        dist_x = std::uniform_int_distribution<uint16_t>(x0, x1 - PLAYER_RADIUS);
        dist_y = std::uniform_int_distribution<uint16_t>(y0, y1 - PLAYER_RADIUS);
    }

    while (true) {
        std::vector<uint16_t> pos = {dist_x(gen), dist_y(gen)};
        auto cell = get_cell_from_position(pos);
        auto ady = get_adyacent_objects(cell);

        Player temp_player(id, pos, player_type, config.get_player_health(),
                           config.get_player_money(), weapon_shop);
        auto collision = collides(temp_player, pos, ady);
        if (collision.first == ObjectType::UNKNOWN) {
            return pos;
        }
    }
}

void Game::set_bomb_player() {
    /*
        Elige al azar un jugador terrorista para que tenga la bomba.
    */
    std::vector<std::uint16_t> terrorists_ids;
    for (auto& player_it: players) {
        if (player_it.second && player_it.second->get_player_type() == PlayerType::TERRORIST) {
            terrorists_ids.push_back(player_it.second->get_id());
        }
    }

    if (!terrorists_ids.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, terrorists_ids.size() - 1);

        auto bomb_carrier_id = terrorists_ids[dis(gen)];
        auto player_it = players.find(bomb_carrier_id);
        if (player_it != players.end()) {
            player_it->second->set_bomb();
        }
    }
}

/*******************************************************************************************
 ************************************BOMBA Y DESACTIVACIÓN**********************************
 ******************************************************************************************/

bool Game::is_in_bomb_zone(const std::vector<uint16_t>& position) const {
    /*
        Devuelve si el jugador está dentro de la zona donde puede plantar la bomba.
    */
    const auto& bomb_zones = map.get_bomb_zones();
    return std::any_of(bomb_zones.begin(), bomb_zones.end(), [this, &position](const auto& zone) {
        return this->circle_rectangle_collision(position, PLAYER_RADIUS,
                                                std::vector<uint16_t>{zone.x, zone.y}, zone.width,
                                                zone.height);
    });
}

bool Game::plant_bomb(const std::vector<uint16_t>& player_position) {
    /*
        Planta una bomba en la posición del jugador con el ID especificado.
        Devuelve true si se planta la bomba, false si no se puede.
    */

    auto bomb_ptr = create_bomb(player_position);

    if (!bomb_ptr) {
        return false;  // No se pudo crear la bomba
    }

    bomb->start_countdown();

    return true;
}

bool Game::deactivate_bomb(const std::vector<uint16_t>& player_position) {
    /*
        Desactiva la bomba si el jugador está a distancia < DEACTIVATION_DISTANCE.
        Devuelve true si se desactiva la bomba, false si no se puede.
    */
    if (bomb) {
        std::vector<uint16_t> bomb_position = bomb->get_position();
        if (distance_between(player_position, bomb_position) < DEACTIVATION_DISTANCE) {
            delete_bomb();
            deactivated = true;
            return true;  // Bomba desactivada
        }
    }
    return false;  // No hay bomba para desactivar
}

void Game::update_bomb_countdown() {
    /*
        Actualiza el temporizador de la bomba y verifica si ha explotado.
    */
    if (bomb)
        exploded = bomb->update_countdown();
}

std::shared_ptr<Bomb> Game::create_bomb(const std::vector<uint16_t>& position) {
    auto bomb_ptr = std::make_shared<Bomb>(position);

    // Agrega la bomba en la matriz
    auto cell = get_cell_from_position(position);
    matrix[cell.first][cell.second].push_back(bomb_ptr);

    // Agrega la bomba al vector de objetos
    objects.push_back(bomb_ptr);

    bomb = bomb_ptr;

    return bomb_ptr;
}

void Game::delete_bomb() {
    /*
        Elimina la bomba de la matriz y del vector de objetos.
    */
    remove_from_matrix(ObjectType::BOMB, bomb->get_id(), bomb->get_position());
    remove_from_objects(ObjectType::BOMB, bomb->get_id());

    // Limpiar referencia
    bomb = nullptr;
}

void Game::remove_from_matrix(ObjectType type, uint16_t id, std::vector<uint16_t> position) {
    /*
        Elimina un objeto de la matriz en la posición dada.
    */
    auto cell = get_cell_from_position(position);
    auto& vec = matrix[cell.first][cell.second];
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [type, id](const std::shared_ptr<Object>& o) {
                                 return o->get_type() == type && o->get_id() == id;
                             }),
              vec.end());
}

void Game::remove_from_objects(ObjectType type, uint16_t id) {
    /*
        Elimina un objeto del vector de objetos.
    */
    objects.erase(std::remove_if(objects.begin(), objects.end(),
                                 [type, id](const std::shared_ptr<Object>& o) {
                                     return o->get_type() == type && o->get_id() == id;
                                 }),
                  objects.end());
}
