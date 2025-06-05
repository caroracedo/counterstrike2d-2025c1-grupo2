#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 200           // posición máxima en la matriz (celdas)
#define MOVE_STEP 5               // paso de movimiento
#define CELL_SIZE 56              // tamaño de cada celda en la matriz
#define PLAYER_RADIUS 16          // radio del jugador
#define BULLET_RADIUS 2           // radio de la bala
#define DEACTIVATION_DISTANCE 50  // distancia máxima para desactivar la bomba

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
#include "weapon_DTO.h"
#include "weapon_shop.h"

class Game {
private:
    std::vector<std::vector<std::vector<std::shared_ptr<Object>>>> matrix;
    std::vector<std::shared_ptr<Object>> objects;
    std::map<uint16_t, std::shared_ptr<Player>> players;  // Mapa de jugadores por ID
    std::map<uint16_t, std::shared_ptr<Player>> dead_players;
    std::map<uint16_t, std::shared_ptr<Bullet>> bullets;  // Mapa de balas por ID
    std::shared_ptr<Bomb> bomb;
    uint16_t bullet_id = 1;
    Config& config;
    WeaponShop weapon_shop;
    bool exploded = false;
    bool deactivated = false;

    /***************************************************************************************************
     *****************************************POSICIONES Y
     *CELDAS***************************************
     ***************************************************************************************************/

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


    /***************************************************************************************************
     ******************************************* MOVIMIENTO
     *********************************************
     ***************************************************************************************************/

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


    /***************************************************************************************************
     *********************************ACTUALIZACIÓN DE JUGADORES Y
     *BALAS********************************
     ***************************************************************************************************/

    // Encuentra el jugador con el ID especificado y le inflige daño, devuelve si el jugador sigue
    // vivo.
    bool damage_player(uint16_t id, uint16_t damage);

    // // Recolecta objetos que no están en la matriz y los elimina de la lista de objetos.
    // void reap();

    // Crea una bala y la agrega a la matriz, al vector de objetos y al mapa de balas.
    void create_bullet(const std::vector<uint16_t>& player_position, const WeaponDTO& weapon_dto,
                       const std::vector<uint16_t>& desired_position);

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
    bool shoot_m3(const std::vector<uint16_t>& player_position, const WeaponDTO& weapon_dto,
                  const std::vector<uint16_t>& desired_position);

    // Dispara 3 balas en ráfaga (una atrás de la otra)
    bool shoot_ak47(const std::vector<uint16_t>& player_position, const WeaponDTO& weapon_dto,
                    const std::vector<uint16_t>& desired_position);

    void employ_knife(const std::vector<uint16_t>& player_position, const WeaponDTO& weapon_dto,
                      const std::vector<uint16_t>& desired_position);


    /***************************************************************************************************
     *******************************************INICIALIZACION******************************************
     ***************************************************************************************************/

    // Inicializa los jugadores con posiciones aleatorias dentro del mapa.
    Player get_player_with_random_position(PlayerType player_type, uint16_t id);

    // Elige al azar un jugador terrorista para que tenga la bomba.
    void set_bomb_player();


    /***************************************************************************************************
     ************************************BOMBA Y
     *DESACTIVACIÓN******************************************
     ***************************************************************************************************/

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
    explicit Game(Config& config);

    // Verifica que haya por lo menos un jugador de cada tipo.
    bool is_ready_to_start();

    // Devuelve el vector de objetos del juego.
    std::vector<std::shared_ptr<Object>>& get_objects();

    // Agrega un jugador al juego con el tipo y ID especificados.
    void add_player(PlayerType player_type, uint16_t id);

    // Mueve al jugador con el ID especificado en la dirección dada.
    bool move(Direction direction, const uint16_t& id);

    // Dispara una bala desde la posición del jugador hacia la posición deseada.
    bool shoot(const std::vector<uint16_t>& position, const uint16_t player_id);

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
    bool shop_ammo(uint ammo, uint16_t id);

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
    }

    void show_objects() {
        std::cout << "Objects:" << std::endl;
        for (const auto& obj: objects) {
            std::cout << "\tID: " << obj->get_id()
                      << ", Type: " << get_object_type(static_cast<ObjectType>(obj->get_type()))
                      << ", Position: (" << obj->get_position()[0] << ", " << obj->get_position()[1]
                      << ")" << std::endl;
        }
        std::cout << std::endl;
    }

    // void show_bullets() {
    //     std::cout << "Bullets:" << std::endl;
    //     for (const auto& [id, bullet]: bullets) {
    //         std::cout << "\tID: " << id << ", Position: (" << bullet->get_position()[0] << ", "
    //                   << bullet->get_position()[1] << "), Range: " << bullet->get_range()
    //                   << ", Damage: " << bullet->get_damage() << std::endl;
    //     }
    //     std::cout << std::endl;
    // }

    std::vector<uint16_t> get_player_position(uint16_t id) const {
        /*
         *    Obtiene la posición del jugador con el ID especificado.
         *    Si no se encuentra, devuelve un vector vacío.
         */
        auto it = players.find(id);
        if (it != players.end()) {
            return it->second->get_position();
        }
        return {};
    }

    std::string get_object_type(const ObjectType& type) const {
        switch (type) {
            case ObjectType::PLAYER:
                return "Player";
            case ObjectType::BULLET:
                return "Bullet";
            case ObjectType::OBSTACLE:
                return "Obstacle";
            default:
                return "Unknown";
        }
    }

    std::string direction_to_string(Direction direction) const {
        /*
         *    Convierte la dirección a una cadena de texto.
         */
        switch (direction) {
            case Direction::UP:
                return "up";
            case Direction::DOWN:
                return "down";
            case Direction::LEFT:
                return "left";
            case Direction::RIGHT:
                return "right";
            default:
                return "unknown";
        }
    }

    void update() { update_bullets(); }
};

#endif  // GAME_H
