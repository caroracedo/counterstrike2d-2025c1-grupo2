#ifndef MONITOR_GAME_H
#define MONITOR_GAME_H

#include <condition_variable>
#include <mutex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "action_DTO.h"
// #include "client_handler.h"  // Acá hay una dependencia circular, por qué en MonitorGame, no
// debería estar en cliente/servidor?
#include "game.h"
#include "queue.h"

class MonitorGame {
private:
    Game game;
    std::mutex mutex;

    // para poder hacer un broadcast
    // std::set<ClientHandler*> players;

    // queue con eventos que van mandando los clientes -> los eventos deberían indicar
    // qué cliente los envió para que el juego se actualice acorde
    Queue<ActionDTO> events_queue;

public:
    /* Mover */
    bool move(Direction direction) {
        std::unique_lock<std::mutex> lock(mutex);
        if (!game.move(direction))
            return false;
        return true;
    }

    /* Mover */
    std::vector<uint8_t> get_position() {
        std::unique_lock<std::mutex> lock(mutex);
        return game.get_position();
    }

    /* Ingresar */
    bool join() {
        std::unique_lock<std::mutex> lock(mutex);
        return true;  // Por ahora no pienso en una cantidad específica de jugadores o cosas así...
    }

    void server_game_loop() {
        init_resources();  //-> parte gráfica
        while (true) {
            receive_updates_from_clients();   // pop de las queues entrantes
            update_game_logic_and_physics();  // cambios en lógica y mapa -> debería recibir por
                                              // parametro el evento y el agente
            broadcast_updates_to_clients();   // les envia a todos los clientes las actualizaciones
            // it = sleep_and_calc_next_it(FPS, x);
        }
    }

    void init_resources() {}

    void receive_updates_from_clients() {
        // ActionDTO action = events_queue.pop();
        // ¿como identifico qué cliente envió esto?
        // devuelvo la accion y quien la hizo
    }

    void update_game_logic_and_physics() {
        // actualizacion del escenario -> Game
        // ¿se podía mover?
        // ¿donde esta ahora?
    }

    void broadcast_updates_to_clients() {
        // les envio a todos los clientes el escenario (Game) actualizado
    }
};

#endif  // MONITOR_GAME_H
