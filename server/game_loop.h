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
        switch (action_dto.type) {
            case ActionType::MOVE:
                return do_move_action(action_dto);
            default:
                return false;
        }
    }

    bool do_move_action(const ActionDTO& action_dto) {
        monitor_game.move(action_dto.direction);
        for (auto* queue: send_queues) {
            queue->push({ActionType::MOVE, monitor_game.get_position()});
        }
        return true;
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
