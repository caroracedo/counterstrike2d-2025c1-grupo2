// #include <iostream>
// #include <iostream>

// #include "game.h"
// #include "player.h"
// #include "weapon.h"
// #include "game.h"
// #include "player.h"
// #include "weapon.h"

// #define MOVE_INPUT "mover"
// #define SHOOT_INPUT "disparar"
// #define BOMB_INPUT "bomba"
// #define SHOW_STATS "stats"
// #define PICK_UP "recoger"
// #define DROP_ALL "soltar"
// #define QUIT_INPUT "q"
// #define W_INPUT "w"
// #define A_INPUT "a"
// #define S_INPUT "s"
// #define D_INPUT "d"
// #define MOVE_INPUT "mover"
// #define SHOOT_INPUT "disparar"
// #define BOMB_INPUT "bomba"
// #define SHOW_STATS "stats"
// #define PICK_UP "recoger"
// #define DROP_ALL "soltar"
// #define QUIT_INPUT "q"
// #define W_INPUT "w"
// #define A_INPUT "a"
// #define S_INPUT "s"
// #define D_INPUT "d"

// void show_stats(const Stats& stats) {
//     std::cout << "Kills: " << std::endl;
//     for (const auto& [id, kills]: stats.kills) {
//         std::cout << "  Player ID: " << id << ", Kills: " << kills << std::endl;
//     }
//     std::cout << "Deaths: " << std::endl;
//     for (const auto& [id, deaths]: stats.deaths) {
//         std::cout << "  Player ID: " << id << ", Deaths: " << deaths << std::endl;
//     }
//     std::cout << "Money: " << std::endl;
//     for (const auto& [id, money]: stats.money) {
//         std::cout << "  Player ID: " << id << ", Money: " << money << std::endl;
//     }
//     std::cout << "Last Winner: "
//               << ((stats.last_winner == PlayerType::TERRORIST) ? "Terrorist" :
//               "Counterterrorist") << std::endl;

//     std::cout << "Team A wins: " << stats.team_a_wins << "\n Team B wins: " << stats.team_b_wins
//     << std::endl;
// }

// int main() {
//     Config config("config/config.yaml");
//     Map map("editor/maps/aztec_village_map.yaml");
//     Game game(config, map, 1, 1);

//     game.add_player(PlayerType::COUNTERTERRORIST, PlayerSkin::PHEONIX, 1);
//     game.add_player(PlayerType::TERRORIST, PlayerSkin::ARTICAVENGER, 2);
//     game.start_round_game_phase();
//     game.show();
//     show_stats(game.get_stats());

//     game.shop_weapon(WeaponModel::AK47, 1);
//     game.shop_weapon(WeaponModel::M3, 2);

//     game.change_weapon(1);
//     // game.change_weapon(1);
//     // game.change_weapon(2);
//     game.change_weapon(2);

//     std::vector<uint16_t> player2_pos = game.get_player_position(2);
//     std::vector<uint16_t> player1_pos = game.get_player_position(1);
//     std::vector<uint16_t> player2_pos = game.get_player_position(2);
//     std::vector<uint16_t> player1_pos = game.get_player_position(1);

//     while (true) {
//         std::string input;
//         std::getline(std::cin, input);
//         std::istringstream iss(input);
//     while (true) {
//         std::string input;
//         std::getline(std::cin, input);
//         std::istringstream iss(input);

//         std::string action_input;
//         if (!(iss >> action_input))
//             continue;
//         std::string action_input;
//         if (!(iss >> action_input))
//             continue;

//         if (action_input == QUIT_INPUT) {
//             return 0;
//         } else if (action_input == MOVE_INPUT) {
//             std::string direction_input;
//             int id;
//             if (!(iss >> direction_input >> id))
//                 break;
//             if (direction_input == W_INPUT)
//                 game.move(Direction::UP, id);
//             else if (direction_input == A_INPUT)
//                 game.move(Direction::LEFT, id);
//             else if (direction_input == S_INPUT)
//                 game.move(Direction::DOWN, id);
//             else if (direction_input == D_INPUT)
//                 game.move(Direction::RIGHT, id);
//             else
//                 break;

//             for (int i = 0; i < 100; ++i) {
//                 game.update();
//             }
//         } else if (action_input == SHOOT_INPUT) {
//             float angle;
//             int id;
//             if (!(iss >> id >> angle)) {
//                 std::cout << "Invalid shoot position input." << std::endl;
//                 continue;
//             }
//             game.rotate_player(angle, id);
//             game.shoot(id);
//             for (int i = 0; i < 100; ++i) {
//                 game.update();
//             }
//         } else if (action_input == BOMB_INPUT) {
//             game.interact_with_bomb(2);
//             while (!game.is_over()) {
//                 game.update();
//             }
//             game.show();
//             show_stats(game.get_stats());
//             continue;
//         } else if (action_input == PICK_UP) {
//             game.pick_up_weapon(1);
//         } else {
//             std::cout << "Unknown action: " << action_input << std::endl;
//             continue;
//         }
//         game.show();
//         if (game.is_over()) {
//             std::cout << "\n********************************************" << std::endl;
//             std::cout << "*************** ROUND OVER *****************" << std::endl;
//             std::cout << "********************************************" << std::endl;
//             Stats stats = game.get_stats();
//             show_stats(stats);
//             game.end_round_game_phase();
//             game.start_round_game_phase();
//             game.show();
//             std::cout << "*********************************************" << std::endl;
//         }
//     }
// }
