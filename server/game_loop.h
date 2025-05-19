#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <list>
#include <utility>
#include <vector>

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
            case ActionType::SHOOT:
                return do_shoot_action(action_dto);
            default:
                return false;
        }
    }

    bool do_move_action(const ActionDTO& action_dto) {
        if (!monitor_game.move(action_dto.direction))
            return false;
        for (auto* queue: send_queues) {
            queue->push({ActionType::MOVE, monitor_game.get_position()});
        }
        return true;
    }

    bool do_shoot_action(const ActionDTO& action_dto) {
        std::vector<uint16_t> position =
                action_dto.position.empty() ? monitor_game.get_position() : action_dto.position;
        std::pair<bool, std::vector<uint16_t>> result =
                monitor_game.shoot(position, action_dto.direction, action_dto.weapon);
        if (!result.first)
            return false;

        // Si la bala no llegó a destino aún, encolar el evento de nuevo con la posicion actualizada
        uint16_t new_range = action_dto.weapon.range - MOVE_DELTA;
        if (new_range > 0) {
            WeaponDTO new_weapon = action_dto.weapon;
            new_weapon.range = new_range;

            ActionDTO new_action_dto(action_dto.direction, new_weapon);
            new_action_dto.position = {result.second[0]};

            recv_queue.push(new_action_dto);
        }

        for (auto* queue: send_queues) {
            queue->push({ActionType::SHOOT, result.second});
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
