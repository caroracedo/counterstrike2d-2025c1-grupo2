#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

#include <chrono>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "../common/action_DTO.h"

#define MOVE_INPUT "mover"
#define SHOOT_INPUT "disparar"
#define BOMB_INPUT "bomba"
#define QUIT_INPUT "q"
#define W_INPUT "w"
#define A_INPUT "a"
#define S_INPUT "s"
#define D_INPUT "d"

class MockHandler {
public:
    /* Procesar */
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
        }
        return {};
    }

    bool update_graphics(const ActionDTO& action_update) {
        if (action_update.type == ActionType::UPDATE) {
            for (const auto& object: action_update.objects) {
                switch (object.type) {
                    case ObjectType::PLAYER: {
                        std::cout << "🧍  [ID: " << object.id << "] ";
                        std::cout << "📍 Pos: (" << static_cast<unsigned int>(object.position[0])
                                  << ", " << static_cast<unsigned int>(object.position[1]) << ") ";

                        std::cout << "🔫 Arma: ";
                        if (object.weapon_model == WeaponModel::KNIFE) {
                            std::cout << "🔪 Cuchillo ";
                        } else if (object.weapon_model == WeaponModel::GLOCK) {
                            std::cout << "🔫 Glock ";
                        } else {
                            std::cout << "❓ Desconocida ";
                        }

                        std::cout << "| 🎭 Rol: ";
                        if (object.player_type == PlayerType::TERRORIST) {
                            std::cout << "Terrorista ";
                        } else if (object.player_type == PlayerType::COUNTERTERRORIST) {
                            std::cout << "Counter-Terrorista ";
                        } else {
                            std::cout << "Desconocido ";
                        }

                        std::cout << "| ❤️ Salud: " << object.health;
                        std::cout << " | 💰 Dinero: $" << object.money << std::endl;
                        break;
                    }

                    case ObjectType::OBSTACLE: {
                        std::cout << "🧱  Obstáculo en ";
                        std::cout << "(" << static_cast<unsigned int>(object.position[0]) << ", "
                                  << static_cast<unsigned int>(object.position[1]) << ") "
                                  << static_cast<unsigned int>(object.width) << "x"
                                  << static_cast<unsigned int>(object.height) << std::endl;
                        break;
                    }

                    case ObjectType::BULLET: {
                        std::cout << "💥  Bala en ";
                        std::cout << "(" << static_cast<unsigned int>(object.position[0]) << ", "
                                  << static_cast<unsigned int>(object.position[1]) << ")"
                                  << std::endl;
                        break;
                    }

                    case ObjectType::BOMB: {
                        std::cout << "💣  Bomba ubicada en ";
                        std::cout << "(" << static_cast<unsigned int>(object.position[0]) << ", "
                                  << static_cast<unsigned int>(object.position[1]) << ")"
                                  << "| ⏳ Tiempo restante: " << object.bomb_countdown << std::endl;
                        break;
                    }

                    default: {
                        std::cout << "❓  Objeto desconocido" << std::endl;
                        break;
                    }
                }
            }

            return true;
        }
        return false;
    }
};

#endif  // INPUT_PARSER_H
