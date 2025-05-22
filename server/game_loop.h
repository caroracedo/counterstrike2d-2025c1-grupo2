#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <list>

#include "../common/action_DTO.h"
#include "../common/monitor_game.h"
#include "../common/queue.h"
#include "../common/thread.h"

class GameLoop: public Thread {
private:
    MonitorGame monitor_game;
    Acceptor acceptor;
    Queue<ActionDTO> recv_queue;
    std::list<Queue<ActionDTO>*> send_queues;

    bool do_action(const ActionDTO& action_dto) {
        std::uint16_t id = 0;  // TODO: obtener el id del jugador que envió la acción
        bool updated = false;
        switch (action_dto.type) {
            case ActionType::MOVE:
                // seria mas correcto hacer move_and_get_objects para evitar race con
                updated = monitor_game.move(action_dto.direction, id);
                break;
            default:
                return false;
        }
        if (updated) {
            // Enviar la posición actualizada a todos los clientes
            for (auto* queue: send_queues) {
                queue->push({ActionType::UPDATE, monitor_game.get_objects()});
            }
        }
        return updated;
    }

public:
    explicit GameLoop(const char* port): acceptor(port, recv_queue, send_queues) {}

    void run() override {
        acceptor.start();

        while (should_keep_running()) {
            ActionDTO action;
            if (recv_queue.try_pop(action)) {
                do_action(action);
            }
        }

        acceptor.stop();
        acceptor.join();
    }
};

#endif  // GAME_LOOP_H
