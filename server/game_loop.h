#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <list>

#include "../common/action_DTO.h"
#include "../common/monitor_game.h"
#include "../common/queue.h"
#include "../common/thread.h"

#define MAX_POSITION 50

class GameLoop: public Thread {
private:
    MonitorGame monitor_game;
    Acceptor acceptor;
    Queue<ActionDTO> recv_queue;
    std::list<Queue<ActionDTO>*> send_queues;

    bool do_action(const ActionDTO& action_dto) {
        switch (action_dto.type) {
            case ActionType::MOVE:
                monitor_game.move_object(action_dto.id, action_dto.direction);
                break;
            default:
                return false;
        }
        return true;
    }

    void send_snapshot_to_all_clients() {
        ActionDTO update{ActionType::UPDATE, monitor_game.get_snapshot()};
        for (auto* queue: send_queues) {
            queue->push(update);
        }
    }

public:
    explicit GameLoop(const char* port):
            monitor_game(MAX_POSITION, MAX_POSITION),
            acceptor(port, recv_queue, send_queues, monitor_game) {}

    void run() override {
        acceptor.start();

        using clock = std::chrono::steady_clock;
        auto last_snapshot_time = clock::now();
        auto interval = std::chrono::seconds(2);  // cada 2 segundos

        while (should_keep_running()) {
            ActionDTO action;
            if (recv_queue.try_pop(action))
                do_action(action);

            auto now = clock::now();
            if (now - last_snapshot_time >= interval) {
                send_snapshot_to_all_clients();
                last_snapshot_time = now;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        acceptor.stop();
        acceptor.join();
    }
};

#endif  // GAME_LOOP_H
