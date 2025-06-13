#include <iostream>

#include "game.h"
#include "player.h"
#include "weapon.h"

#define MOVE_INPUT "mover"
#define SHOOT_INPUT "disparar"
#define BOMB_INPUT "bomba"
#define SHOW_STATS "stats"
#define PICK_UP "recoger"
#define QUIT_INPUT "q"
#define W_INPUT "w"
#define A_INPUT "a"
#define S_INPUT "s"
#define D_INPUT "d"

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
    } else if (action_input == PICK_UP) {
        std::vector<uint16_t> position(2);
        return {ActionType::WEAPON, {0, 0}};
    } else {
        std::cerr << "Unknown action: " << action_input << std::endl;
        return {};
    }
    return {};
}

int main() {
    Config config("config/config2.yaml");
    Game game(config);

    game.add_player(PlayerType::COUNTERTERRORIST, 1);
    game.add_player(PlayerType::TERRORIST, 2);
    game.start_round_game_phase();
    game.show_objects();

    game.shop_weapon(WeaponModel::AK47, 1);
    game.shop_weapon(WeaponModel::AWP, 2);

    std::vector<uint16_t> player2_pos = game.get_player_position(2);
    std::vector<uint16_t> player1_pos = game.get_player_position(1);

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
                game.shoot(player2_pos, 1);
                for (int i = 0; i < 100; ++i) {
                    game.update();
                }
                break;
            case ActionType::BOMB:
                std::cout << "interactuando con bomba" << std::endl;
                game.interact_with_bomb(1);
                break;
            case ActionType::QUIT:
                return 0;
            default:
                std::cout << "\tAccion desconocida" << std::endl;
                break;
        }
        game.show_objects();
        if (game.is_over()) {
            std::cout << "Round Over!" << std::endl;
            game.end_round_game_phase();
            game.start_round_game_phase();
        }
    }

    // std::cout << "\n[TEST] Player 2 dropping AWP" << std::endl;
    // game.drop_primary_weapon(2);
    // game.show_objects();

    // std::cout << "\n[TEST] Player 1 picking up AWP => dropping AK-47" << std::endl;
    // game.pick_up_weapon(player2_pos, 1);
    // game.show_objects();

    // std::cout << "\n[TEST] Player 1 dropping all weapons (including bomb)" << std::endl;
    // game.drop_weapons(1);
    // game.show_objects();

    // std::cout << "\n[TEST] Player 1 picking up weapon just dropped" << std::endl;
    // game.pick_up_weapon(player1_pos, 1);
    // game.show_objects();
}
