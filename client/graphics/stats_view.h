#ifndef STATS_VIEW_H
#define STATS_VIEW_H

#include <map>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/stats.h"
#include "common/types.h"

#include "sound_manager.h"

class StatsView {
private:
    SDL2pp::Renderer& renderer;
    SDL2pp::Rect overlay_rect;
    SDL2pp::Font font;
    SDL2pp::Font end_font;
    bool visible = false;
    bool sounds_played = false;
    Stats current_stats;
    std::map<uint16_t, std::string> player_names;
    std::unordered_map<uint16_t, PlayerType> player_types;
    SoundManager& sound_manager;

public:
    explicit StatsView(SDL2pp::Renderer& renderer, SoundManager& sound_manager);

    void update(const Stats& new_stats, const std::unordered_map<uint16_t, PlayerType>& types);

    void render();

    void set_visible(bool visible);

    void render_team_section(PlayerType team, int start_x, int start_y);

    void render_pre_lobby();

    void reset_sounds();

    void render_winner_banner(WinnerTeamType winner);
};

#endif
