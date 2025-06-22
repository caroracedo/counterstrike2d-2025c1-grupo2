#include "match.h"

#include <algorithm>
#include <chrono>
#include <iostream>

#define TIME 1
#define SHOP_TIME 10
#define SNAPSHOT_TIME 33  // ~30FPS
#define STATS_TIME 5
#define WAIT_TIME 100

/* Constructor */
Match::Match(Config& config, std::shared_ptr<Queue<ActionDTO>> recv_queue,
             std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues,
             const std::string& map_str, uint16_t number_terrorist,
             uint16_t number_counterterrorist):
        config(config),
        map(map_str.c_str()),
        game(config, map, number_terrorist, number_counterterrorist),
        recv_queue(recv_queue),
        monitor_client_send_queues(monitor_client_send_queues) {}

/* Manejo de acciones */
bool Match::do_action(const ActionDTO& action_dto) {
    switch (action_dto.type) {
        case ActionType::MOVE:
            return game.move(action_dto.direction, action_dto.id);
        case ActionType::SHOOT:
            return game.shoot(action_dto.id);
        case ActionType::BOMB:
            return game.interact_with_bomb(action_dto.id);
        case ActionType::CHANGE:
            return game.change_weapon(action_dto.id);
        case ActionType::PICKUP:
            game.pick_up_weapon(action_dto.id);
            return true;
        case ActionType::ROTATE:
            game.rotate_player(action_dto.angle, action_dto.id);
            return true;
        case ActionType::QUIT:
            game.quit(action_dto.id);
            return true;
        case ActionType::HEALTHCHEAT:
            game.do_health_cheat(action_dto.id);
            return true;
        case ActionType::AMMOCHEAT:
            game.do_ammo_cheat(action_dto.id);
            return true;
        case ActionType::MONEYCHEAT:
            game.do_money_cheat(action_dto.id);
            return true;
        case ActionType::WINCHEAT:
            game.do_win_cheat(action_dto.id);
            return true;
        default:
            return false;
    }
}

bool Match::do_shop_action(const ActionDTO& action_dto) {
    switch (action_dto.type) {
        case ActionType::WEAPON:
            return game.shop_weapon(action_dto.weapon, action_dto.id);
        case ActionType::AMMOPRIMARY:
        case ActionType::AMMOSECONDARY:
            return game.shop_ammo(action_dto.ammo, action_dto.weapon_type, action_dto.id);
        case ActionType::QUIT:
            game.quit(action_dto.id);
            return true;
        default:
            return false;
    }
}

void Match::do_start_action(const ActionDTO& action_dto) {
    switch (action_dto.type) {
        case ActionType::START:
            return game.set_ready_to_start();
        case ActionType::QUIT:
            return game.quit(action_dto.id);
        default:
            return;
    }
}

/* Envío a todos los clientes */
void Match::send_initial_snapshot_to_all_clients() {
    monitor_client_send_queues->send_update(
            {ActionType::UPDATE, process_objects(game.get_objects())});
}

void Match::send_snapshot_to_all_clients() {
    monitor_client_send_queues->send_update(
            {ActionType::UPDATE, process_dynamic_objects(game.get_objects())});
}

void Match::send_shop_to_all_clients() {
    monitor_client_send_queues->send_update({ActionType::SHOP, config.get_weapons()});
}

void Match::send_end_to_all_clients() {
    monitor_client_send_queues->send_update(ActionDTO(ActionType::END));
}

void Match::send_stats_to_all_clients() {
    monitor_client_send_queues->send_update({ActionType::STATS, game.get_stats()});
}


/* Conversión de Objetos */
std::vector<ObjectDTO> Match::process_objects(const std::vector<std::shared_ptr<Object>>& objects) {
    std::vector<ObjectDTO> object_dtos;
    object_dtos.reserve(objects.size());
    std::transform(objects.begin(), objects.end(), std::back_inserter(object_dtos),
                   [](const std::shared_ptr<Object>& obj_ptr) { return obj_ptr->get_dto(); });
    return object_dtos;
}

std::vector<ObjectDTO> Match::process_dynamic_objects(
        const std::vector<std::shared_ptr<Object>>& objects) {
    std::vector<ObjectDTO> object_dtos;
    for (const auto& obj_ptr: objects) {
        ObjectType type = obj_ptr->get_type();
        if (type != ObjectType::OBSTACLE && type != ObjectType::BOMBZONE)
            object_dtos.push_back(obj_ptr->get_dto());
    }
    return object_dtos;
}


/* Waiting Lobby */
void Match::waiting_lobby() {
    std::cout << "[WAIT] Esperando a que todos los jugadores estén listos..." << std::endl;

    // Nota: Cuáles son las condiciones de corte?:
    //          a. Ya se puede empezar -> se debería continuar
    //          b. Se interrumpe la partida -> no se debería continuar
    //      Ambas se cumplen gracias al inicio de cada ronda.
    // Polling
    while (!game.is_ready_to_start() && should_keep_running()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));

        ActionDTO action;
        if (recv_queue->try_pop(action))
            do_start_action(action);
    }

    send_initial_snapshot_to_all_clients();
    std::cout << "[WAIT] ¡Todos los jugadores están listos!" << std::endl;
}


/* Match Loop */
void Match::match_loop() {
    std::cout << "[MATCH] ¡Que comience la partida!" << std::endl;

    // Nota: Cuáles son las condiciones para comenzar una nueva ronda?:
    //          a. Todavía quedan más rondas
    //          b. La partida no se interrumpió
    //          c. El juego puede seguir en curso
    for (size_t round = 1;
         round <= config.get_rounds_total() && should_keep_running() && !game.is_over(); ++round) {
        std::cout << "[ROUND] Iniciando ronda..." << std::endl;

        shopping_phase();
        game_phase();
        stats_phase();

        std::cout << "[ROUND] Terminando ronda..." << std::endl;

        if (round == config.get_rounds_total() / 2) {
            std::cout << "[SWITCH] Switch de equipos!" << std::endl;
            game.switch_player_types();
        }
    }

    send_end_to_all_clients();
    std::cout << "[MATCH] ¡La partida ha finalizado!" << std::endl;
}

void Match::shopping_phase() {
    std::cout << "[SHOP] Iniciando fase de compras..." << std::endl;
    send_shop_to_all_clients();

    auto shop_time = std::chrono::seconds(SHOP_TIME);
    auto shop_start = std::chrono::steady_clock::now();

    // Nota: Cuáles son las condiciones de corte?:
    //          a. La partida se interrumpió -> no se debería continuar
    //          b. El juego no puede seguir en curso -> no se debería continuar (sólo Stats)
    //          c. Terminó el SHOP_TIME -> se debería continuar
    //      Se cumplen gracias al flujo de ejecución.
    // Polling
    auto now = std::chrono::steady_clock::now();
    while (should_keep_running() && !game.is_over() && (now - shop_start < shop_time)) {
        std::this_thread::sleep_for(std::chrono::seconds(TIME));

        ActionDTO action;
        while (should_keep_running() && !game.is_over() && (now - shop_start < shop_time) &&
               recv_queue->try_pop(action)) {
            do_shop_action(action);

            now = std::chrono::steady_clock::now();
        }

        now = std::chrono::steady_clock::now();
    }

    std::cout << "[SHOP] Terminando fase de compras..." << std::endl;
}

void Match::game_phase() {
    std::cout << "[GAME] Iniciando fase de juego..." << std::endl;
    send_snapshot_to_all_clients();
    game.start_round_game_phase();

    // Nota: Cuáles son las condiciones de corte?:
    //          a. La partida se interrumpió -> no se debería continuar
    //          b. El juego no puede seguir en curso -> no se debería continuar (sólo Stats)
    //      Se cumplen gracias al flujo de ejecución.
    auto snapshot_interval = std::chrono::milliseconds(SNAPSHOT_TIME);
    auto last_snapshot_time = std::chrono::steady_clock::now();
    while (should_keep_running() && !game.is_over()) {
        auto start = std::chrono::steady_clock::now();

        auto now = start;
        ActionDTO action;
        while (should_keep_running() && !game.is_over() && (now - start < snapshot_interval) &&
               recv_queue->try_pop(action)) {
            do_action(action);
            now = std::chrono::steady_clock::now();
        }

        if (now - last_snapshot_time >= snapshot_interval) {
            send_snapshot_to_all_clients();
            last_snapshot_time = now;
        }

        auto elapsed = now - start;
        if (elapsed < snapshot_interval)
            std::this_thread::sleep_for(snapshot_interval - elapsed);
    }

    send_snapshot_to_all_clients();
    game.end_round_game_phase();
    std::cout << "[GAME] Terminando fase de juego..." << std::endl;
}

void Match::stats_phase() {
    std::cout << "[STATS] Iniciando fase de estadísticas..." << std::endl;

    send_stats_to_all_clients();

    auto stats_time = std::chrono::seconds(STATS_TIME);
    auto stats_start = std::chrono::steady_clock::now();

    // Nota: Cuáles son las condiciones de corte?:
    //          a. La partida se interrumpió -> no se debería continuar
    //          b. Terminó el SHOP_TIME -> se debería continuar
    //      Se cumplen gracias al flujo de ejecución.
    // Polling
    auto now = std::chrono::steady_clock::now();
    while (should_keep_running() && (now - stats_start < stats_time)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));

        ActionDTO action;
        if (recv_queue->try_pop(action) && action.type == ActionType::QUIT)
            game.quit(action.id);

        now = std::chrono::steady_clock::now();
    }

    std::cout << "[STATS] Terminando fase de estadísticas..." << std::endl;
}

/* Override */
void Match::run() {
    waiting_lobby();
    match_loop();
}

/* Añadir jugador */
void Match::add_player(PlayerType player_type, PlayerSkin player_skin, uint16_t id) {
    std::lock_guard<std::mutex> lock(mutex);
    game.add_player(player_type, player_skin, id);
}

/* Getters */
TerrainType Match::get_terrain() {
    std::lock_guard<std::mutex> lock(mutex);
    return map.get_terrain();
}

/* Validación */
bool Match::is_started() {
    std::lock_guard<std::mutex> lock(mutex);
    return game.is_ready_to_start();
}
