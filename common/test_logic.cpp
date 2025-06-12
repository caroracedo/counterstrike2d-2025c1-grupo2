#include "game.h"
#include "player.h"
#include "weapon.h"

int main() {
    Config config("config/config2.yaml");
    Game game(config);

    game.add_player(PlayerType::TERRORIST, 1);
    game.add_player(PlayerType::COUNTERTERRORIST, 2);
    game.start_round_game_phase();

    game.shop_weapon(WeaponModel::AK47, 1);
    game.shop_weapon(WeaponModel::AWP, 2);

    WeaponDTO awp = game.drop_primary_weapon(2);
    game.pick_up_weapon(awp, 1);
    game.drop_weapons(1);
}
