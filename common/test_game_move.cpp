#include <iostream>

#include "action_DTO.h"
#include "config.h"
#include "game.h"

#define MOVE_INPUT "mover"
#define SHOOT_INPUT "disparar"
#define BOMB_INPUT "bomba"
#define SHOW_STATS "stats"
#define QUIT_INPUT "q"
#define W_INPUT "w"
#define A_INPUT "a"
#define S_INPUT "s"
#define D_INPUT "d"

void show(Game& game) { game.show_objects(); }

ActionDTO receive_and_parse_action() {
    std::string input;
    std::getline(std::cin, input);
    std::istringstream iss(input);

    std::string action_input;
    if (!(iss >> action_input))
        return {};

    if (action_input == QUIT_INPUT) {
        return ActionDTO(ActionType::QUIT);
    } else if (action_input == MOVE_INPUT) {
        std::string direction_input;
        if (!(iss >> direction_input))
            return {};
        Direction direction;
        if (direction_input == W_INPUT)
            direction = Direction::UP;
        else if (direction_input == A_INPUT)
            direction = Direction::LEFT;
        else if (direction_input == S_INPUT)
            direction = Direction::DOWN;
        else if (direction_input == D_INPUT)
            direction = Direction::RIGHT;
        else
            return {};

        return {ActionType::MOVE, direction};
    } else if (action_input == SHOOT_INPUT) {
        std::vector<uint16_t> desired_position(2);
        if (!(iss >> desired_position[0] >> desired_position[1]))
            return {};
        return {ActionType::SHOOT, desired_position};
    } else if (action_input == BOMB_INPUT) {
        return ActionDTO(ActionType::BOMB);
    } else if (action_input == SHOW_STATS) {
        return ActionDTO(ActionType::UPDATE);
    }
    return {};
}

int main() {
    Config config("config/config.yaml");
    Game game(config);

    game.add_player(PlayerType::TERRORIST, 1);
    game.add_player(PlayerType::COUNTERTERRORIST, 2);
    game.start_round_game_phase();
    show(game);

    std::vector<uint16_t> player2_position = game.get_player_position(2);

    while (true) {
        ActionDTO action = receive_and_parse_action();
        switch (action.type) {
            case ActionType::MOVE:
                game.move(action.direction, 1);
                for (int i = 0; i < 100; ++i) {
                    game.update();
                }
                break;
            case ActionType::SHOOT:
                game.shoot(1, player2_position);
                for (int i = 0; i < 100; ++i) {
                    game.update();
                }
                break;
            case ActionType::BOMB:
                std::cout << "interactuando con bomba" << std::endl;
                game.interact_with_bomb(1);
                break;
            case ActionType::UPDATE:
                // Ahora lo uso para poder ver las stats
                game.show_stats();
                break;
            case ActionType::QUIT:
                return 0;
            default:
                std::cout << "\tAccion desconocida" << std::endl;
                break;
        }
        show(game);
        if (game.is_over()) {
            std::cout << "Round Over!" << std::endl;
            game.end_round_game_phase();
            game.show_stats();
            game.start_round_game_phase();
        }
    }

    return 0;
}
