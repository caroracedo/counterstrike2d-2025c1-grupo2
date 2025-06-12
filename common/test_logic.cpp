#include <iostream>

#include "game.h"
#include "player.h"
#include "weapon.h"

int main() {
    Config config("config/config2.yaml");
    Game game(config);

    game.add_player(PlayerType::TERRORIST, 1);
    game.add_player(PlayerType::COUNTERTERRORIST, 2);
    game.start_round_game_phase();
    game.show_objects();

    game.shop_weapon(WeaponModel::AK47, 1);
    game.shop_weapon(WeaponModel::AWP, 2);

    std::cout << "\n[TEST] Player 2 dropping AWP" << std::endl;
    WeaponDTO awp = game.drop_primary_weapon(2);
    game.show_objects();

    std::cout << "\n[TEST] Player 1 picking up AWP => dropping AK-47" << std::endl;
    game.pick_up_weapon(awp, 1);
    game.show_objects();

    std::cout << "\n[TEST] Player 1 dropping all weapons (including bomb)" << std::endl;
    game.drop_weapons(1);
    game.show_objects();
}
