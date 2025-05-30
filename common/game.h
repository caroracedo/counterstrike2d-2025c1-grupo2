#ifndef GAME_H
#define GAME_H

#define MATRIX_SIZE 200  // posición máxima en la matriz
#define MOVE_STEP 5      // paso de movimiento
#define CELL_SIZE 56     // tamaño de cada celda en la matriz
#define PLAYER_SIZE 32   // tamaño del jugador
#define BULLET_SIZE 8    // tamaño de la bala

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "action_DTO.h"
#include "object.h"
#include "object_DTO.h"
#include "obstacle.h"
#include "player.h"
#include "types.h"
#include "weapon.h"

class Game {
private:
    std::vector<std::vector<std::vector<std::shared_ptr<Object>>>> matrix;
    std::vector<std::shared_ptr<Object>> objects;
    std::map<uint16_t, std::shared_ptr<Player>> players;  // Mapa de jugadores por ID
    std::map<uint16_t, std::shared_ptr<Bullet>> bullets;  // Mapa de balas por ID
    uint16_t bullet_id = 1;

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
                                                 std::vector<uint16_t> new_position);

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
    bool find_player_and_damage(uint16_t id, uint16_t damage);

    // Recolecta objetos que no están en la matriz y los elimina de la lista de objetos.
    void reap();

    // Incrementa el ID de la siguiente bala.
    void inc_bullet_id();

    // Calcula la posición inicial de la bala en función de la dirección y la posición del jugador.
    std::vector<uint16_t> calculate_bullet_starting_position(
            const std::vector<uint16_t>& player_position,
            const std::vector<uint16_t>& desired_position);

    // Actualiza las balas en la matriz, eliminando las que ya no están activas.
    void update_bullets();

    uint16_t get_damage_and_delete_bullet(uint bullet_id);

public:
    Game();

    bool move(Direction direction, const uint16_t& id);

    bool shoot(const std::vector<uint16_t>& desired_position, const uint16_t& player_id);

    std::vector<std::shared_ptr<Object>>& get_objects() {
        update_bullets();
        return objects;
    }

    void add_player(const uint16_t& id, const bool is_terrorist, const bool has_bomb) {
        auto player1 = std::make_shared<Player>(
                id, std::vector<uint16_t>{0, 0},
                is_terrorist ? PlayerType::TERRORIST : PlayerType::COUNTERTERRORIST, has_bomb);
        // Agregar el jugador a players
        players.insert({id, player1});

        // Agregar el jugador a objects
        objects.push_back(player1);

        // Agregar el jugador a la matriz
        auto cell = get_cell_from_position(player1->get_position());
        matrix[cell.first][cell.second].push_back(player1);
    }

    /********************************************************************************************
     ************************************ FUNCIONES PARA TESTEAR ********************************
     ********************************************************************************************/
    void initialize_demo_objects() {
        // Jugador en (10, 10)
        auto player1 = std::make_shared<Player>(1, std::vector<uint16_t>{10, 10},
                                                PlayerType::TERRORIST, true);
        players.insert({1, player1});
        objects.push_back(player1);
        auto cell = get_cell_from_position(player1->get_position());
        matrix[cell.first][cell.second].push_back(player1);

        // Obstáculo 1 en (50, 10)
        auto obstacle1 = std::make_shared<Obstacle>(std::vector<uint16_t>{50, 10}, 6, 6);
        objects.push_back(obstacle1);
        cell = get_cell_from_position(obstacle1->get_position());
        matrix[cell.first][cell.second].push_back(obstacle1);

        // Bala en (45, 50)
        auto bullet = std::make_shared<Bullet>(7, std::vector<uint16_t>{45, 50}, 10, 20);
        bullets.insert({7, bullet});
        objects.push_back(bullet);
        cell = get_cell_from_position(bullet->get_position());
        matrix[cell.first][cell.second].push_back(bullet);

        // Otro jugador en (80, 80)
        auto player2 = std::make_shared<Player>(2, std::vector<uint16_t>{80, 80},
                                                PlayerType::COUNTERTERRORIST, false);
        players.insert({2, player2});
        objects.push_back(player2);
        cell = get_cell_from_position(player2->get_position());
        matrix[cell.first][cell.second].push_back(player2);

        // Otro jugador en (30, 80)
        auto player3 = std::make_shared<Player>(3, std::vector<uint16_t>{30, 80},
                                                PlayerType::COUNTERTERRORIST, false);
        players.insert({3, player3});
        objects.push_back(player3);
        cell = get_cell_from_position(player3->get_position());
        matrix[cell.first][cell.second].push_back(player3);
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

    void show_bullets() {
        std::cout << "Balas:" << std::endl;
        for (const auto& bullet_pair: bullets) {
            const auto& bullet = bullet_pair.second;
            std::cout << "\tID: " << bullet->get_id() << ", Posición: ("
                      << bullet->get_position()[0] << ", " << bullet->get_position()[1] << ")"
                      << ", Rango: " << bullet->get_range() << ", Daño: " << bullet->get_damage()
                      << std::endl;
        }
        std::cout << std::endl;
    }

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

    std::vector<uint16_t> get_bullet_starting_position(
            const std::vector<uint16_t>& player_position,
            const std::vector<uint16_t>& desired_position) {
        return calculate_bullet_starting_position(player_position, desired_position);
    }
};

#endif  // GAME_H
