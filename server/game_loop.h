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
        std::vector<ObjectDTO> game_update;
        switch (action_dto.type) {
            case ActionType::MOVE:
                game_update = monitor_game.move_object(action_dto.id, action_dto.direction);
                for (auto* queue: send_queues) {
                    queue->push({ActionType::UPDATE,
                                 game_update});  // TODO: Mandar siempre el game_update?
                }
                break;
            default:
                return false;
        }
        return true;
    }

public:
    explicit GameLoop(const char* port):
            monitor_game(MAX_POSITION, MAX_POSITION),
            acceptor(port, recv_queue, send_queues, monitor_game) {}

    void run() override {
        acceptor.start();

        while (should_keep_running()) {
            ActionDTO action;
            if (recv_queue.try_pop(action))
                do_action(action);
        }

        acceptor.stop();
        acceptor.join();
    }
};

#endif  // GAME_LOOP_H
