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

    /*******************************************************************************************
     *****************************************POSICIONES Y CELDAS*******************************
     ******************************************************************************************/

    // Obtiene la celda correspondiente a una posición dada.
    std::pair<uint16_t, uint16_t> get_cell_from_position(const std::vector<uint16_t>& position);

    // Obtiene las celdas adyacentes a la celda dada dentro de un radio especificado.
    std::vector<std::pair<uint16_t, uint16_t>> get_cells_within_radius(
            const std::pair<uint16_t, uint16_t>& cell, uint16_t radius);

    // Calcula la distancia entre dos posiciones dadas.
    uint16_t distance_between(const std::vector<uint16_t>& position1,
                              const std::vector<uint16_t>& position2);

    // Obtiene los objetos en las celdas adyacentes a la celda dada.
    std::set<std::shared_ptr<Object>> get_adyacent_objects(
            const std::pair<uint16_t, uint16_t>& cell);


    /*******************************************************************************************
     ***************************************** MOVIMIENTO **************************************
     ******************************************************************************************/

    // Realiza el movimiento del objeto dado en la dirección dada.
    std::pair<bool, std::vector<uint16_t>> _move(const Object& obj,
                                                 const std::vector<uint16_t>& new_position);

    // Determina la posición máxima a la que se puede mover el objeto sin colisionar.
    std::vector<uint16_t> get_max_position(const Object& obj,
                                           const std::vector<uint16_t>& new_position);

    // Verifica si hay colisiones con otros objetos en la nueva posición.
    std::pair<ObjectType, uint16_t> collides(const Object& object,
                                             const std::vector<uint16_t>& new_position,
                                             const std::set<std::shared_ptr<Object>>&);

    // Devuelve si hay colisión entre dos círculos dados sus posiciones y radios.
    bool circle_circle_collision(const std::vector<uint16_t>& new_position,
                                 const uint16_t& object_radius,
                                 const std::vector<uint16_t>& collider_position,
                                 const uint16_t& collider_radius);

    // Devuelve si hay colisión entre un círculo y un rectángulo dados sus posiciones y dimensiones.
    bool circle_rectangle_collision(const std::vector<uint16_t>& c_center, const uint16_t& c_radius,
                                    const std::vector<uint16_t>& r_position,
                                    const uint16_t& r_width, const uint16_t& r_height) const;

    // Actualiza la posición del objeto en la matriz. Devuelve true si se actualizó correctamente.
    bool update_object_in_matrix(const std::shared_ptr<Object>& obj,
                                 const std::vector<uint16_t>& old_position);

    // Maneja la colisión entre una bala y un jugador. Devuelve true si el jugador sigue vivo.
    bool handle_bullet_player_collision(const uint16_t& bullet_id, const uint16_t& shooter_id,
                                        const uint16_t& player_id);


    /*******************************************************************************************
     *********************************ACTUALIZACIÓN DE JUGADORES Y BALAS************************
     ******************************************************************************************/

    // Encuentra el jugador con el ID especificado y le inflige daño, devuelve si el jugador sigue
    // vivo.
    bool damage_player(uint16_t id, uint16_t damage);

    // Crea una bala y la agrega a la matriz, al vector de objetos y al mapa de balas.
    void create_bullet(const std::vector<uint16_t>& player_position, const u_int16_t player_id,
                       const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);

    // Devuelve el daño infligido por la bala y elimina la bala de la matriz, del vector de objetos
    // y del mapa de balas.
    uint16_t get_damage_and_delete_bullet(const uint16_t& bullet_id);

    // Elimina la bala con el ID especificado de la matriz, del vector de objetos y del mapa de
    // balas.
    void delete_bullet(const uint16_t& bullet_id);

    // Incrementa el ID de la siguiente bala.
    void inc_bullet_id();

    // Actualiza las balas en la matriz, eliminando las que ya no están activas.
    void update_bullets();

    // Dispara 3 balas en cono
    bool shoot_m3(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                  const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);

    // Dispara 3 balas en ráfaga (una atrás de la otra)
    bool shoot_ak47(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                    const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);

    // Actualiza las ráfagas de AK-47, eliminando las que ya no están activas y disparando las que
    // aún tienen disparos restantes.
    void update_ak47_bursts();

    // Emplea el cuchillo para atacar a un jugador en la posición deseada.
    void employ_knife(const std::vector<uint16_t>& player_position, const uint16_t player_id,
                      const WeaponDTO& weapon_dto, const std::vector<uint16_t>& desired_position);


    /*******************************************************************************************
     *******************************************INICIALIZACION**********************************
     ******************************************************************************************/
    // Inicializa los obstáculos y zonas de bomba del juego a partir de la configuración.
    void initialize_objects();

    // Devuelve una posición aleatoria dentro del mapa para un jugador de tipo dado.
    std::vector<uint16_t> get_random_player_position(PlayerType player_type, uint16_t id);

    // Elige al azar un jugador terrorista para que tenga la bomba.
    void set_bomb_player();


    /*******************************************************************************************
     ************************************BOMBA Y DESACTIVACIÓN**********************************
     ******************************************************************************************/

    // Devuelve si el jugador está dentro de la zona donde puede plantar la bomba.
    bool is_in_bomb_zone(const std::vector<uint16_t>& position) const;

    // Crea la bomba en la posición del jugador y la agrega a los objetos y a la matriz.
    bool plant_bomb(const std::vector<uint16_t>& player_position);

    // Desactiva la bomba si el jugador es un counter-terrorista y está dentro del rango de
    // desactivación. Devuelve true si se desactiva correctamente, false en caso contrario.
    bool deactivate_bomb(const std::vector<uint16_t>& player_position);

    // Actualiza el temporizador de la bomba y verifica si ha explotado.
    void update_bomb_countdown();

    // Elimina la bomba de la matriz y del vector de objetos.
    void delete_bomb();

public:
    explicit Game(Config& config, Map& map);

    // Devuelve si hay jugadores suficientes en cada equipo.
    bool is_ready_to_start();

    // Devuelve el vector de objetos del juego.
    std::vector<std::shared_ptr<Object>>& get_objects();

    // Agrega un jugador al juego con el tipo y ID especificados.
    void add_player(PlayerType player_type, uint16_t id);

    // Mueve al jugador con el ID especificado en la dirección dada.
    bool move(Direction direction, const uint16_t& id);

    // Dispara desde la posición del jugador hacia la posición deseada, con el arma actual del
    // jugador.
    bool shoot(const std::vector<uint16_t>& desired_position, const uint16_t player_id);

    // Interactúa con la bomba: planta si es terrorista o desactiva si es counter-terrorista.
    bool interact_with_bomb(const uint16_t& player_id);

    // Indica si el juego ha terminado, es decir, si todos los jugadores de un equipo han muerto, si
    // explotó la bomba o si se desactivó.
    bool is_over();

    // Inicia la fase de juego de la ronda, eligiendo al jugador que tendrá la bomba.
    void start_round_game_phase();

    // Reinicia la fase de juego de la ronda, eliminando la bomba si existe y reestableciendo los
    // jugadores muertos.
    void end_round_game_phase();

    // Cambia el tipo de todos los jugadores en el juego, de terrorista a counter-terrorista y
    // viceversa.
    void switch_player_types();

    // Permite a los jugadores comprar un arma en la fase inicial
    bool shop_weapon(WeaponModel weapon, uint16_t id);

    // Permite a los jugadores comprar munición en la fase inicial
    bool shop_ammo(uint ammo, WeaponType weapon, uint16_t id);

    // Permite al jugador con el ID especificado cambiar de arma.
    bool change_weapon(uint16_t id);

    Stats get_stats() const { return stats; }

    void set_ready_to_start() { _is_ready_to_start = true; }


    // FUNCIONES EN DESARROLLO

    WeaponDTO drop_primary_weapon(uint16_t id) {
        auto players_it = players.find(id);
        if (players_it == players.end()) {
            std::cout << "[GAME] Player with ID " << id << " not found.\n";
            return WeaponDTO();
        }
        WeaponDTO weapon = players_it->second->drop_primary_weapon();
        create_weapon(weapon, players_it->second->get_position());
        return weapon;
    }

    WeaponDTO drop_weapons(uint16_t id) {
        auto players_it = players.find(id);
        if (players_it == players.end()) {
            std::cout << "[GAME] Player with ID " << id << " not found.\n";
            return WeaponDTO();
        }
        auto result = players_it->second->drop_weapons();
        std::cout << "[GAME] Player with ID " << id
                  << " dropped weapons. Primary: " << result.first.id
                  << ", Bomb: " << (result.second ? "Yes" : "No") << "\n";
        std::vector<uint16_t> position = players_it->second->get_position();
        std::vector<uint16_t> weapon_position = {
                static_cast<uint16_t>(position[0] - PLAYER_RADIUS),
                static_cast<uint16_t>(position[1] - PLAYER_RADIUS)};
        create_weapon(result.first, weapon_position);
        if (result.second) {
            std::vector<uint16_t> bomb_position = {static_cast<uint16_t>(position[0] + 8),
                                                   static_cast<uint16_t>(position[1] + 8)};
            create_bomb(bomb_position);
        }
        return result.first;
    }

    void pick_up_weapon(uint16_t id) {
        std::cout << "[GAME] Player with ID: " << id << "trying to pick up weapon.\n";
        auto players_it = players.find(id);
        if (players_it == players.end()) {
            std::cout << "[GAME] Player with ID " << id << " not found.\n";
            return;
        }
        std::vector<uint16_t> player_position = players_it->second->get_position();
        auto cell = get_cell_from_position(player_position);
        auto adyacent_objects = get_adyacent_objects(cell);
        auto player_ptr = players_it->second;

        for (const auto& obj: adyacent_objects) {
            if (obj->get_type() == ObjectType::WEAPON) {
                bool is_near_weapon = circle_rectangle_collision(
                        player_position, PLAYER_RADIUS + 1, obj->get_position(), obj->get_width(),
                        obj->get_height());
                if (is_near_weapon) {
                    WeaponDTO new_weapon_dto = delete_weapon(obj->get_id());
                    WeaponDTO old_weapon_dto = player_ptr->pick_up_weapon(new_weapon_dto);
                    std::cout << "[GAME] Player with ID " << id
                              << " picked up weapon with ID: " << new_weapon_dto.id << "\n";
                    if (old_weapon_dto.model != WeaponModel::UNKNOWN) {
                        create_weapon(old_weapon_dto, player_position);
                        std::cout << "[GAME] Player with ID " << id
                                  << " dropped old weapon with ID: " << old_weapon_dto.id << "\n";
                    }
                    return;
                }
            } else if (obj->get_type() == ObjectType::BOMB &&
                       distance_between(player_position, obj->get_position()) <=
                               PLAYER_RADIUS + BOMB_RADIUS + 1) {
                std::cout << "[GAME] Player with ID " << id
                          << " trying to pick up a bomb at position: (" << player_position[0]
                          << ", " << player_position[1] << ")" << std::endl;
                WeaponDTO bomb_dto;
                bomb_dto.model = WeaponModel::BOMB;
                player_ptr->pick_up_weapon(bomb_dto);
                if (player_ptr->can_plant_bomb()) {
                    delete_bomb();
                    return;
                }
            }
        }
    }


    void create_weapon(const WeaponDTO& weapon_dto, const std::vector<uint16_t>& position) {
        Weapon weapon(weapon_dto);
        weapon.move(position);

        auto weapon_ptr = std::make_shared<Weapon>(weapon);
        // Agrega el arma al vector de armas
        weapons[weapon_dto.id] = weapon_ptr;

        // Agrega el arma en la matriz
        auto cell = get_cell_from_position(position);
        matrix[cell.first][cell.second].push_back(weapon_ptr);

        // Agrega el arma al vector de objetos
        objects.push_back(weapon_ptr);
    }

    WeaponDTO delete_weapon(const uint16_t weapon_id) {
        std::cout << "[GAME] Deleting weapon with ID: " << weapon_id << "\n";
        auto it = weapons.find(weapon_id);
        if (it != weapons.end()) {
            auto weapon_ptr = it->second;
            WeaponDTO weapon_dto = weapon_ptr->get_weapon_dto();
            // Elimina el arma de la matriz
            auto cell = get_cell_from_position(weapon_ptr->get_position());
            auto& vec = matrix[cell.first][cell.second];
            vec.erase(std::remove(vec.begin(), vec.end(), weapon_ptr), vec.end());
            // Elimina el arma del vector de objetos
            objects.erase(std::remove(objects.begin(), objects.end(), weapon_ptr), objects.end());
            // Elimina del mapa de weapons
            weapons.erase(it);
            return weapon_dto;
        }
        return WeaponDTO();
    }

    std::shared_ptr<Bomb> create_bomb(const std::vector<uint16_t>& position) {
        auto bomb_ptr = std::make_shared<Bomb>(position);

        // Agrega la bomba en la matriz
        auto cell = get_cell_from_position(position);
        matrix[cell.first][cell.second].push_back(bomb_ptr);

        // Agrega la bomba al vector de objetos
        objects.push_back(bomb_ptr);

        bomb = bomb_ptr;

        return bomb_ptr;
    }

    void rotate_player(uint16_t id, float angle) {
        auto player_it = players.find(id);
        if (player_it != players.end()) {
            player_it->second->rotate(angle);
        } else {
            std::cout << "[GAME] Player with ID " << id << " not found.\n";
        }
    }

    // void show_objects() {
    //     for (const auto& obj: objects) {
    //         if (obj->get_type() == ObjectType::WEAPON) {
    //             std::cout << "[GAME] Weapon: " << obj->get_dto().get_weapon_model() << " "
    //                       << obj->get_id() << " at position: " << obj->get_position()[0] << ", "
    //                       << obj->get_position()[1] << "\n";
    //         } else if (obj->get_type() == ObjectType::PLAYER) {
    //             std::cout << "[GAME] Player ID: " << obj->get_id()
    //                       << " at position: " << obj->get_position()[0] << ", "
    //                       << obj->get_position()[1] << "\n";
    //         } else if (obj->get_type() == ObjectType::BOMB) {
    //             std::cout << "[GAME] Bomb at position: " << obj->get_position()[0] << ", "
    //                       << obj->get_position()[1] << "\n";
    //         }
    //     }
    // }

    std::vector<uint16_t> get_player_position(uint16_t id) {
        auto player_it = players.find(id);
        if (player_it != players.end()) {
            return player_it->second->get_position();
        }
        return {};
    }

    void update() { update_bullets(); }
};

#endif  // GAME_H
