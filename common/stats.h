#ifndef STATS_H
#define STATS_H

#include <cstdint>
#include <map>

#include "types.h"

struct Stats {
    std::map<uint16_t, uint16_t> kills;            // ID del jugador -> cantidad de asesinatos
    std::map<uint16_t, uint16_t> deaths;           // ID del jugador -> cantidad de muertes
    std::map<uint16_t, uint16_t> money;            // ID del jugador -> dinero recolectado
    PlayerType last_winner = PlayerType::UNKNOWN;  // Último ganador de la ronda
    uint16_t team_a_wins = 0;
    uint16_t team_b_wins = 0;
};

#endif  // STATS_H
