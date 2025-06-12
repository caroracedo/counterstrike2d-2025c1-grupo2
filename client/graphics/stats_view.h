#ifndef STATS_VIEW_H
#define STATS_VIEW_H

#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

#include <SDL2pp/SDL2pp.hh>

#include "common/constants.h"
#include "common/stats.h"
#include "common/types.h"

class StatsView {
private:
    SDL2pp::Renderer& renderer;

    SDL2pp::Rect overlay_rect;

    SDL2pp::Font font;

    bool visible = false;

    Stats current_stats;

    std::map<uint16_t, std::string> player_names;
    std::unordered_map<uint16_t, PlayerType> player_types;


public:
    explicit StatsView(SDL2pp::Renderer& renderer):
            renderer(renderer),
            overlay_rect(SCREEN_MARGIN, SCREEN_MARGIN, SCREEN_WIDTH - (SCREEN_MARGIN * 2),
                         SCREEN_HEIGHT - (SCREEN_MARGIN * 2)),
            font(ASSETS_PATH "/gfx/fonts/korean.ttf", 16) {}


    void update(const Stats& new_stats, const std::unordered_map<uint16_t, PlayerType>& types) {
        current_stats = new_stats;

        player_types = types;

        for (const auto& [id, type]: player_types) {
            player_names[id] = "player " + std::to_string(id);
        }

        for (const auto& [id, d]: new_stats.deaths) {
            std::cout << "ID: " << id << " deaths: " << d << std::endl;
        }
    }

    void render() {
        if (!visible)
            return;

        SDL2pp::Color prevColor = renderer.GetDrawColor();

        renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(17, 17, 17, 200);

        renderer.FillRect(overlay_rect);

        renderer.SetDrawColor(prevColor);

        int x = overlay_rect.GetX() + 20;
        int y = overlay_rect.GetY() + 20;
        int x2 = ((SCREEN_WIDTH - (SCREEN_MARGIN * 2)) / 2) + 30;
        int w = (SCREEN_WIDTH - (SCREEN_MARGIN * 5) - 10) / 2;

        renderer.SetDrawColor(50, 50, 200, 200);
        renderer.FillRect(SDL2pp::Rect{x, y, w, 20});

        renderer.SetDrawColor(255, 50, 50, 200);
        renderer.FillRect(SDL2pp::Rect{x2, y, w, 20});

        // draw sections...

        render_team_section(PlayerType::TERRORIST, x2, y);
        render_team_section(PlayerType::COUNTERTERRORIST, x, y);
    }

    void set_visible(bool visible) { this->visible = visible; }

    void render_team_section(PlayerType team, int start_x, int start_y) {
        std::string title =
                (team == PlayerType::TERRORIST) ? "Terrorist Forces" : "Counter-Terrorist Forces";
        SDL_Color title_color = {255, 255, 255, 255};
        SDL_Color text_color = (team == PlayerType::TERRORIST) ? SDL_Color{200, 50, 50, 255} :
                                                                 SDL_Color{50, 50, 200, 255};

        // Render título
        SDL2pp::Texture title_tex(renderer, font.RenderText_Solid(title, title_color));
        renderer.Copy(title_tex, SDL2pp::NullOpt,
                      SDL2pp::Rect(start_x, start_y, title_tex.GetWidth(), title_tex.GetHeight()));
        start_y += title_tex.GetHeight() + 10;

        // Coordenadas para columnas
        int name_x = start_x;
        int kills_x = name_x + 104;
        int deaths_x = kills_x + 60;
        int money_x = deaths_x + 60;

        // Render encabezado
        SDL2pp::Texture name_header(renderer, font.RenderText_Solid("Player", text_color));
        SDL2pp::Texture kills_header(renderer, font.RenderText_Solid("Kills", text_color));
        SDL2pp::Texture deaths_header(renderer, font.RenderText_Solid("Deaths", text_color));
        SDL2pp::Texture money_header(renderer, font.RenderText_Solid("$", text_color));

        int header_y = start_y;
        renderer.Copy(
                name_header, SDL2pp::NullOpt,
                SDL2pp::Rect(name_x, header_y, name_header.GetWidth(), name_header.GetHeight()));
        renderer.Copy(
                kills_header, SDL2pp::NullOpt,
                SDL2pp::Rect(kills_x, header_y, kills_header.GetWidth(), kills_header.GetHeight()));
        renderer.Copy(deaths_header, SDL2pp::NullOpt,
                      SDL2pp::Rect(deaths_x, header_y, deaths_header.GetWidth(),
                                   deaths_header.GetHeight()));
        renderer.Copy(
                money_header, SDL2pp::NullOpt,
                SDL2pp::Rect(money_x, header_y, money_header.GetWidth(), money_header.GetHeight()));
        start_y += name_header.GetHeight() + 5;

        // Render stats de cada jugador
        for (const auto& [id, type]: player_types) {
            if (type != team)
                continue;

            std::string name =
                    player_names.count(id) ? player_names.at(id) : "Player " + std::to_string(id);
            uint16_t kills = current_stats.kills.count(id) ? current_stats.kills.at(id) : 0;
            uint16_t deaths = current_stats.deaths.count(id) ? current_stats.deaths.at(id) : 0;
            uint16_t money = current_stats.money.count(id) ? current_stats.money.at(id) : 0;
            SDL2pp::Texture name_tex(renderer, font.RenderText_Solid(name, text_color));
            SDL2pp::Texture kills_tex(renderer,
                                      font.RenderText_Solid(std::to_string(kills), text_color));
            SDL2pp::Texture deaths_tex(renderer,
                                       font.RenderText_Solid(std::to_string(deaths), text_color));
            SDL2pp::Texture money_tex(
                    renderer, font.RenderText_Solid("$" + std::to_string(money), text_color));


            renderer.Copy(name_tex, SDL2pp::NullOpt,
                          SDL2pp::Rect(name_x, start_y, name_tex.GetWidth(), name_tex.GetHeight()));
            renderer.Copy(
                    kills_tex, SDL2pp::NullOpt,
                    SDL2pp::Rect(kills_x, start_y, kills_tex.GetWidth(), kills_tex.GetHeight()));
            renderer.Copy(
                    deaths_tex, SDL2pp::NullOpt,
                    SDL2pp::Rect(deaths_x, start_y, deaths_tex.GetWidth(), deaths_tex.GetHeight()));
            renderer.Copy(
                    money_tex, SDL2pp::NullOpt,
                    SDL2pp::Rect(money_x, start_y, money_tex.GetWidth(), money_tex.GetHeight()));
        }
    }
};

#endif
