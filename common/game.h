#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "object_DTO.h"

class Game {  // Una implementación simple para testear
private:
    std::vector<std::vector<ObjectDTO*>> grid;
    std::unordered_map<int, ObjectDTO*> id_players;

    bool is_valid(int x, int y) {
        return y >= 0 && y < (int)grid.size() && x >= 0 && x < (int)grid[0].size();
    }

public:
    Game(int width, int height) { grid.resize(height, std::vector<ObjectDTO*>(width, nullptr)); }

    void add_player(int id) {
        ObjectDTO* new_player = new ObjectDTO(ObjectType::PLAYER, {0, 0}, id);
        id_players[id] = new_player;
        grid[new_player->position[0]][new_player->position[1]] = new_player;
    }

    std::vector<ObjectDTO> move_object(int objectId, Direction dir) {
        if (id_players.find(objectId) == id_players.end())
            return {};

        ObjectDTO* obj = id_players[objectId];
        int x = obj->position[0];
        int y = obj->position[1];
        int new_x = x, new_y = y;

        switch (dir) {
            case Direction::UP:
                new_y--;
                break;
            case Direction::DOWN:
                new_y++;
                break;
            case Direction::LEFT:
                new_x--;
                break;
            case Direction::RIGHT:
                new_x++;
                break;
            default:
                return {};  // dirección inválida
        }

        if (!is_valid(new_x, new_y))
            return {};
        if (grid[new_y][new_x] != nullptr)
            return {};  // colisión

        // Mover
        grid[y][x] = nullptr;
        grid[new_y][new_x] = obj;
        obj->position[0] = new_x;
        obj->position[1] = new_y;
        return get_objects();
    }

    std::vector<ObjectDTO> get_objects() {
        std::vector<ObjectDTO> objects;
        for (const auto& [id, playerPtr]: id_players) {
            if (playerPtr != nullptr) {
                objects.push_back(*playerPtr);  // <-- copiás el objeto, no el puntero
            }
        }
        return objects;
    }
};

#endif  // GAME_H
