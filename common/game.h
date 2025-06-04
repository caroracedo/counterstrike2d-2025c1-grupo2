#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 200   // posición máxima en la matriz (celdas)
#define MOVE_STEP 5       // paso de movimiento
#define CELL_SIZE 56      // tamaño de cada celda en la matriz
#define PLAYER_RADIUS 16  // radio del jugador
#define BULLET_RADIUS 4   // radio de la bala

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
#include "config.h"
#include "object.h"
#include "object_DTO.h"
#include "obstacle.h"
#include "player.h"
#include "types.h"
#include "weapon.h"
#include "weapon_shop.h"

class Game {
private:
    std::vector<std::vector<std::vector<std::shared_ptr<Object>>>> matrix;
    std::vector<std::shared_ptr<Object>> objects;
    std::map<uint16_t, std::shared_ptr<Player>> players;  // Mapa de jugadores por ID
    std::map<uint16_t, std::shared_ptr<Bullet>> bullets;  // Mapa de balas por ID
    std::shared_ptr<Bomb> bomb;
    uint16_t bullet_id = 1;
    Config& config;
    WeaponShop weapon_shop;
    bool exploded = false;

    // Obtiene la celda correspondiente a una posición dada.
    std::pair<uint16_t, uint16_t> get_cell_from_position(const std::vector<uint16_t>& position);

    // Obtiene las celdas adyacentes a la celda dada dentro de un radio especificado.
    std::vector<std::pair<uint16_t, uint16_t>> get_cells_within_radius(
            const std::pair<uint16_t, uint16_t>& cell, uint16_t radius);


    // Obtiene los objetos en las celdas adyacentes a la celda dada.
    std::set<std::shared_ptr<Object>> get_adyacent_objects(
            const std::pair<uint16_t, uint16_t>& cell);

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

    // Actualiza la posición del objeto en la matriz. Devuelve true si se actualizó correctamente.
    bool update_object_in_matrix(const std::shared_ptr<Object>& obj,
                                 const std::vector<uint16_t>& old_position);

    // Encuentra el jugador con el ID especificado y le inflige daño, devuelve si el jugador sigue
    // vivo.
    bool damage_player(uint16_t id, uint16_t damage);

    // // Recolecta objetos que no están en la matriz y los elimina de la lista de objetos.
    // void reap();

    // Crea una bala y la agrega a la matriz, al vector de objetos y al mapa de balas.
    void create_bullet(const std::vector<uint16_t>& player_position, const uint16_t& range,
                       const uint16_t& damage, const std::vector<uint16_t>& desired_position);

    // Elimina la bala con el ID especificado de la matriz, del vector de objetos y del mapa de
    // balas.
    void delete_bullet(const uint16_t& bullet_id);

    // Devuelve el daño infligido por la bala y elimina la bala de la matriz, del vector de objetos
    // y del mapa de balas.
    uint16_t get_damage_and_delete_bullet(const uint16_t& bullet_id);

    // Incrementa el ID de la siguiente bala.
    void inc_bullet_id();

    // Actualiza las balas en la matriz, eliminando las que ya no están activas.
    void update_bullets();

    // Dispara 3 balas en cono
    void shoot_m3(const std::vector<uint16_t>& player_position, uint16_t range, uint16_t damage,
                  const std::vector<uint16_t>& desired_position);

    // Dispara 3 balas en ráfaga (una atrás de la otra)
    void shoot_ak47(const std::vector<uint16_t>& player_position, uint16_t range, uint16_t damage,
                    const std::vector<uint16_t>& desired_position);

    Player get_player_with_random_position(PlayerType player_type, uint16_t id);

    void set_bomb_player();

    void update_bomb_countdown();

    void delete_bomb();

public:
    explicit Game(Config& config);

    bool move(Direction direction, const uint16_t& id);

    bool shoot(const std::vector<uint16_t>& position, const uint16_t player_id);

    bool plant_bomb(const uint16_t& player_id);

    std::vector<std::shared_ptr<Object>>& get_objects();

    void add_player(PlayerType player_type, uint16_t id);

    bool is_over();

    bool is_ready_to_start();

    bool shop_weapon(WeaponModel weapon, uint16_t id) {
        auto player_it = players.find(id);
        if (player_it != players.end()) {
            return player_it->second->buy_weapon(weapon);
        }
        return false;
    }

    bool shop_ammo(uint ammo, uint16_t id) {
        auto player_it = players.find(id);
        if (player_it != players.end()) {
            return player_it->second->buy_ammo(ammo);
        }
        return false;
    }

    /********************************************************************************************
     ************************************ FUNCIONES PARA TESTEAR ********************************
     ********************************************************************************************/
    void initialize_demo_objects() {
        for (const auto& obs_cfg: config.get_obstacles()) {
            auto obstacle = std::make_shared<Obstacle>(std::vector<uint16_t>{obs_cfg.x, obs_cfg.y},
                                                       obs_cfg.width, obs_cfg.height);
            objects.push_back(obstacle);

            auto cell = get_cell_from_position(obstacle->get_position());
            matrix[cell.first][cell.second].push_back(obstacle);
        }
        //  // Jugador en (30, 30)
        // auto player1 = std::make_shared<Player>(1, std::vector<uint16_t>{30, 30},
        //                                         PlayerType::TERRORIST, false, weapon_shop);
        // player1->buy_weapon(WeaponModel::M3);
        // player1->change_weapon();
        // player1->change_weapon();
        // std::cout << "Arma actual del jugador 1: " << player1->get_current_weapon_name()
        //             << std::endl;
        // players.insert({1, player1});
        // objects.push_back(player1);
        // auto cell = get_cell_from_position(player1->get_position());
        // matrix[cell.first][cell.second].push_back(player1);


        // // Obstáculo 1 en (50, 10)
        // auto obstacle1 = std::make_shared<Obstacle>(std::vector<uint16_t>{50, 10}, 30, 6);
        // objects.push_back(obstacle1);
        // cell = get_cell_from_position(obstacle1->get_position());
        // matrix[cell.first][cell.second].push_back(obstacle1);

        // // // Bala en (45, 50)
        // // auto bullet = std::make_shared<Bullet>(7, std::vector<uint16_t>{45, 50}, 10, 20);
        // // bullets.insert({7, bullet});
        // // objects.push_back(bullet);
        // // cell = get_cell_from_position(bullet->get_position());
        // // matrix[cell.first][cell.second].push_back(bullet);

        // // Otro jugador en (80, 80)
        // auto player2 = std::make_shared<Player>(2, std::vector<uint16_t>{80, 80},
        //                                         PlayerType::COUNTERTERRORIST, false,
        //                                         weapon_shop);
        // players.insert({2, player2});
        // objects.push_back(player2);
        // cell = get_cell_from_position(player2->get_position());
        // matrix[cell.first][cell.second].push_back(player2);

        // // Otro jugador en (30, 80)
        // auto player3 = std::make_shared<Player>(3, std::vector<uint16_t>{30, 80},
        //                                         PlayerType::COUNTERTERRORIST, false,
        //                                         weapon_shop);
        // players.insert({3, player3});
        // objects.push_back(player3);
        // cell = get_cell_from_position(player3->get_position());
        // matrix[cell.first][cell.second].push_back(player3);
    }

    void show_objects() {
        std::cout << "Objetos:" << std::endl;
        for (const auto& obj: objects) {
            std::cout << "\tID: " << obj->get_id()
                      << ", Tipo: " << get_object_type(static_cast<ObjectType>(obj->get_type()))
                      << ", Posición: (" << obj->get_position()[0] << ", " << obj->get_position()[1]
                      << ")" << std::endl;
        }
        std::cout << std::endl;
    }

    // void show_bullets() {
    //     std::cout << "Balas:" << std::endl;
    //     for (const auto& [id, bullet]: bullets) {
    //         std::cout << "\tID: " << id << ", Posición: (" << bullet->get_position()[0] << ", "
    //                   << bullet->get_position()[1] << "), Rango: " << bullet->get_range()
    //                   << ", Daño: " << bullet->get_damage() << std::endl;
    //     }
    //     std::cout << std::endl;
    // }

    std::vector<uint16_t> get_position(uint16_t id) const {
        auto it = std::find_if(
                objects.begin(), objects.end(),
                [id](const std::shared_ptr<Object>& obj) { return obj->get_id() == id; });
        if (it != objects.end()) {
            return (*it)->get_position();
        }
        return {};
    }

    std::string get_object_type(const ObjectType& type) const {
        switch (type) {
            case ObjectType::PLAYER:
                return "Jugador";
            case ObjectType::BULLET:
                return "Bala";
            case ObjectType::OBSTACLE:
                return "Obstáculo";
            default:
                return "Desconocido";
        }
    }

    std::string direction_to_string(Direction direction) const {
        /*
         *    Convierte la dirección a una cadena de texto.
         */
        switch (direction) {
            case Direction::UP:
                return "arriba";
            case Direction::DOWN:
                return "abajo";
            case Direction::LEFT:
                return "izquierda";
            case Direction::RIGHT:
                return "derecha";
            default:
                return "desconocida";
        }
    }

    uint16_t distance_moved(const std::vector<uint16_t>& old_position,
                            const std::vector<uint16_t>& new_position) {
        /*
         *    Calcula la distancia recorrida entre dos posiciones.
         */
        return static_cast<uint16_t>(std::sqrt(std::pow(new_position[0] - old_position[0], 2) +
                                               std::pow(new_position[1] - old_position[1], 2)));
    }

    void update() { update_bullets(); }
};

#endif  // GAME_H
