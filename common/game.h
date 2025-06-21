#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <random>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "bomb.h"
#include "bomb_zone.h"
#include "config.h"
#include "constants.h"
#include "knife.h"
#include "map.h"
#include "object.h"
#include "object_DTO.h"
#include "obstacle.h"
#include "player.h"
#include "stats.h"
#include "types.h"
#include "weapon.h"
#include "weapon_DTO.h"
#include "weapon_shop.h"

struct Ak47Burst {
    std::vector<uint16_t> player_position;
    uint16_t player_id;
    WeaponDTO weapon_dto;
    std::vector<uint16_t> desired_position;
    int shots_left;
    float time_until_next_shot;  // en segundos
};

class Game {
private:
    std::vector<std::vector<std::vector<std::shared_ptr<Object>>>> matrix;
    std::vector<std::shared_ptr<Object>> objects;
    std::map<uint16_t, std::shared_ptr<Player>> players;  // Mapa de jugadores por ID
    std::map<uint16_t, std::shared_ptr<Player>> dead_players;
    std::map<uint16_t, std::shared_ptr<Bullet>> bullets;  // Mapa de balas por ID
    std::map<uint16_t, std::shared_ptr<Weapon>> weapons;  // Mapa de armas por ID
    std::shared_ptr<Bomb> bomb;
    uint16_t bullet_id = 1;
    int round_number = 0;
    Config& config;
    Map& map;
    WeaponShop weapon_shop;
    bool exploded = false;
    bool deactivated = false;
    Stats stats;
    std::vector<Ak47Burst> ak47_bursts;
    bool _is_ready_to_start = false;
    uint16_t number_terrorist;
    uint16_t number_counterterrorist;

    std::pair<uint16_t, uint16_t> get_cell_from_position(const std::vector<uint16_t>& position);

    std::vector<std::pair<uint16_t, uint16_t>> get_cells_within_radius(
            const std::pair<uint16_t, uint16_t>& cell, uint16_t radius);

    uint16_t distance_between(const std::vector<uint16_t>& position1,
                              const std::vector<uint16_t>& position2);

    std::set<std::shared_ptr<Object>> get_adyacent_objects(
            const std::pair<uint16_t, uint16_t>& cell);

    void remove_from_matrix(ObjectType type, uint16_t id, std::vector<uint16_t> position);

    void remove_from_objects(ObjectType type, uint16_t id);

    /*******************************************************************************************
     ***************************************** MOVIMIENTO **************************************
     ******************************************************************************************/

    std::pair<bool, std::vector<uint16_t>> _move(const Object& obj,
                                                 const std::vector<uint16_t>& new_position);

    std::vector<uint16_t> get_max_position(const Object& obj,
                                           const std::vector<uint16_t>& new_position);

    std::pair<ObjectType, uint16_t> collides(const Object& object,
                                             const std::vector<uint16_t>& new_position,
                                             const std::set<std::shared_ptr<Object>>&);

    bool circle_circle_collision(const std::vector<uint16_t>& new_position,
                                 const uint16_t& object_radius,
                                 const std::vector<uint16_t>& collider_position,
                                 const uint16_t& collider_radius);

    bool circle_rectangle_collision(const std::vector<uint16_t>& c_center, const uint16_t& c_radius,
                                    const std::vector<uint16_t>& r_position,
                                    const uint16_t& r_width, const uint16_t& r_height) const;

    bool update_object_in_matrix(const std::shared_ptr<Object>& obj,
                                 const std::vector<uint16_t>& old_position);

    bool handle_bullet_player_collision(const uint16_t& bullet_id, const uint16_t& shooter_id,
                                        const uint16_t& player_id);


    /*******************************************************************************************
     *********************************ACTUALIZACIÓN DE JUGADORES Y BALAS************************
     ******************************************************************************************/

    bool damage_player(uint16_t id, uint16_t damage);

    void create_bullet(const std::vector<uint16_t>& player_position, const u_int16_t player_id,
                       const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);

    uint16_t get_damage_and_delete_bullet(const uint16_t& bullet_id);

    void delete_bullet(const uint16_t& bullet_id);

    void inc_bullet_id();

    void update_bullets();

    bool shoot_m3(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                  const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);

    bool shoot_ak47(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                    const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);

    void update_ak47_bursts();

    void employ_knife(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                      const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);

    void create_weapon(const WeaponDTO& weapon_dto, const std::vector<uint16_t>& position);

    WeaponDTO delete_weapon(const uint16_t weapon_id);

    void drop_weapons(uint16_t id);

    /*******************************************************************************************
     *******************************************INICIALIZACION**********************************
     ******************************************************************************************/
    void initialize_objects();

    std::vector<uint16_t> get_random_player_position(PlayerType player_type, PlayerSkin player_skin,
                                                     uint16_t id);

    void set_bomb_player();


    /*******************************************************************************************
     ************************************BOMBA Y DESACTIVACIÓN**********************************
     ******************************************************************************************/

    bool is_in_bomb_zone(const std::vector<uint16_t>& position) const;

    bool plant_bomb(const std::vector<uint16_t>& player_position);

    bool deactivate_bomb(const std::vector<uint16_t>& player_position);

    void update_bomb_countdown();

    std::shared_ptr<Bomb> create_bomb(const std::vector<uint16_t>& position);

    void delete_bomb();

public:
    explicit Game(Config& config, Map& map, uint16_t number_terrorist,
                  uint16_t number_counterterrorist);

    bool is_ready_to_start();

    void set_ready_to_start();

    std::vector<std::shared_ptr<Object>>& get_objects();

    void add_player(PlayerType player_type, PlayerSkin player_skin, uint16_t id);

    bool move(Direction direction, const uint16_t& id);

    void rotate_player(float angle, uint16_t id);

    bool shoot(const std::vector<uint16_t>& desired_position, const uint16_t player_id);

    void pick_up_weapon(uint16_t id);

    bool interact_with_bomb(const uint16_t& player_id);

    bool is_over();

    void start_round_game_phase();

    void end_round_game_phase();

    void switch_player_types();

    bool shop_weapon(WeaponModel weapon, uint16_t id);

    bool shop_ammo(uint ammo, WeaponType weapon, uint16_t id);

    bool change_weapon(uint16_t id);

    Stats get_stats() const;

    void quit(uint16_t id);
};

#endif  // GAME_H
