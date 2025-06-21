#ifndef MATCH_H
#define MATCH_H

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "common/action_DTO.h"
#include "common/object_DTO.h"
#include "common/queue.h"
#include "common/thread.h"

#include "monitor_client_send_queues.h"
#include "monitor_game.h"

class Match: public Thread {
private:
    /* Configuración */
    Config& config;
    Map map;
    MonitorGame monitor_game;
    std::shared_ptr<Queue<ActionDTO>> recv_queue;
    std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues;
    std::mutex mutex;

    /* Manejo de acciones */
    bool do_action(const ActionDTO& action_dto);
    bool do_shop_action(const ActionDTO& action_dto);
    void do_start_action(const ActionDTO& action_dto);

    /* Envío a todos los clientes */
    void send_initial_snapshot_to_all_clients();
    void send_snapshot_to_all_clients();
    void send_shop_to_all_clients();
    void send_end_to_all_clients();
    void send_stats_to_all_clients();

    /* Conversión de Objetos */
    std::vector<ObjectDTO> process_objects(const std::vector<std::shared_ptr<Object>>& objects);
    std::vector<ObjectDTO> process_dynamic_objects(
            const std::vector<std::shared_ptr<Object>>& objects);

    /* Waiting Lobby */
    void waiting_lobby();

    /* Match Loop */
    void match_loop();
    void shopping_phase();
    void game_phase();
    void stats_phase();

public:
    /* Constructor */
    Match(Config& config, std::shared_ptr<Queue<ActionDTO>> recv_queue,
          std::shared_ptr<MonitorClientSendQueues> monitor_client_send_queues,
          const std::string& map_str, uint16_t number_terrorist, uint16_t number_counterterrorist);

    /* Override */
    void run() override;

    /* Añadir jugador */
    void add_player(PlayerType player_type, PlayerSkin player_skin, uint16_t id);

    /* Getters */
    TerrainType get_terrain();

    /* Validación */
    bool is_started();
};

#endif  // MATCH_H
