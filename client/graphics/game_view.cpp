#include "game_view.h"

#include <limits>
#include <unordered_set>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

GameView::GameView():
        sdl(SDL_INIT_VIDEO),
        window("CS2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
               SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        texture_manager(renderer),
        camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2048, 2048),
        hud_view(renderer, *texture_manager.get_texture("hud_numbers"),
                 *texture_manager.get_texture("hud_life"),
                 *texture_manager.get_texture("hud_money")),
        bomb_view(*texture_manager.get_texture("bomb"), *texture_manager.get_texture("explotion"),
                  sound_manager),
        shop_view(renderer, sound_manager),
        stats_view(renderer, sound_manager),
        fov_view(renderer, texture_manager) {
    SDL_ShowCursor(SDL_DISABLE);
    init_terrains();
    int diag = static_cast<int>(
            std::ceil(std::sqrt(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT)));
    fov_view.generate_FOV_texture(diag, diag, 64, 90);
}

void GameView::init_terrains() {
    terrains[TerrainType::PUEBLOAZTECA] = "aztec";
    terrains[TerrainType::ZONAENTRENAMIENTO] = "office";
    terrains[TerrainType::DESIERTO] = "sand1";
}


void GameView::set_id(uint16_t id) { local_id = id; }

GameCamera& GameView::get_camera() { return camera; }

ShopView& GameView::get_shop() { return shop_view; }

void GameView::waiting_lobby(bool flag) { show_waiting_lobby = flag; }

void GameView::set_terrain(TerrainType type) { terrain = type; }

bool GameView::can_player_attack() { return players[local_id]->can_attack(); }

void GameView::handle_shop_action() {
    bomb_view.reset();
    shop_view.set_visible(true);
    stats_view.set_visible(false);
    bomb_view.reset_sounds();
    sounds_played = false;
    is_first_update = false;
    is_alive = true;
}

void GameView::handle_stats_action(const ActionDTO& action) {
    stats_view.update(action.stats, types);
    stats_view.set_visible(true);
    stats_view.reset_sounds();
}
void GameView::update(const ActionDTO& action) {
    if (action.type == ActionType::SHOP) {
        handle_shop_action();
        return;
    }

    if (action.type == ActionType::STATS) {
        handle_stats_action(action);
        return;
    }

    if (!sounds_played && !is_first_update) {
        sound_manager.play("go", 0);
        sounds_played = true;
    }

    int bombs_in_match = 0;

    shop_view.set_visible(false);
    stats_view.set_visible(false);

    if (is_alive)
        fov_view.set_visible(true);

    bullets.clear();
    drops.clear();

    std::unordered_set<uint8_t> players_in_game;
    for (const auto& object: action.objects) {

        if (object.type == ObjectType::PLAYER) {
            players_in_game.insert(object.id);
            update_player(object);
        } else if (object.type == ObjectType::OBSTACLE) {
            update_obstacles(object);
        } else if (object.type == ObjectType::BULLET) {
            update_bullets(object);
        } else if (object.type == ObjectType::BOMB) {
            bombs_in_match++;
            update_bomb(object);
        } else if (object.type == ObjectType::BOMBZONE) {
            update_bombzone(object);
        } else if (object.type == ObjectType::WEAPON) {
            update_drops(object);
        }
    }

    if (bombs_in_match == 0) {
        bomb_view.reset();
        bomb_view.set_dropped(false);
    }

    for (auto it = players.begin(); it != players.end();) {
        if (players_in_game.find(it->first) == players_in_game.end()) {
            if (it->first == local_id) {
                fov_view.set_visible(false);
                is_alive = false;
            }
            it = players.erase(it);
        } else {
            ++it;
        }
    }
}

void GameView::update_player(const ObjectDTO& object) {
    uint16_t id = object.id;
    float x = object.position[0];
    float y = object.position[1];

    if (players.find(id) == players.end()) {
        auto player = std::make_unique<PlayerView>(texture_manager, sound_manager, id,
                                                   object.player_skin);
        players[id] = std::move(player);
    }

    types.try_emplace(id, object.player_type);

    if (players[id]->update(object)) {
        if (!is_first_update && is_alive) {
            auto sound_pos = std::make_pair(x, y);
            auto it = players.find(local_id);
            if (it != players.end() && it->second) {
                auto listener_pos = std::make_pair(it->second->get_x(), it->second->get_y());
                sound_manager.play_with_distance("steps_" + std::to_string(rand() % 4 + 1), 0, 600,
                                                 sound_pos, listener_pos, 350);
            }
        }
    }

    if (id == local_id) {
        hud_view.update(object);
    }
}

void GameView::update_bullets(const ObjectDTO& object) {
    BulletView bullet(object.position[0], object.position[1]);
    bullets.push_back(bullet);
}

void GameView::update_obstacles(const ObjectDTO& object) {
    ObstacleView obs(renderer, texture_manager);

    obs.update(object.position[0], object.position[1], object.width, object.height,
               object.obstacle_type);

    obstacles.push_back(obs);
}

void GameView::update_drops(const ObjectDTO& object) {
    DropView drop_view(renderer, texture_manager);

    drop_view.update(object.position[0], object.position[1], object.weapon_model);

    drops.push_back(drop_view);
}

void GameView::update_bomb(const ObjectDTO& object) {
    if (object.bomb_countdown == 0) {
        bomb_view.explode();
    } else if (object.bomb_countdown == std::numeric_limits<uint16_t>::max()) {
        bomb_view.set_dropped(true);
    } else {
        bomb_view.activate_bomb();
        bomb_view.set_dropped(false);
    }

    hud_view.update_timer(object);
    std::optional<std::pair<float, float>> listener_pos = std::nullopt;
    auto it = players.find(local_id);
    if (it != players.end() && it->second) {
        listener_pos = std::make_pair(it->second->get_x(), it->second->get_y());
    }
    bomb_view.update(object.position[0], object.position[1], object.bomb_countdown, listener_pos);
}

void GameView::update_bombzone(const ObjectDTO& object) {
    SDL_Rect rect;
    rect.x = static_cast<int>(object.position[0]);
    rect.y = static_cast<int>(object.position[1]);
    rect.w = static_cast<int>(object.width);
    rect.h = static_cast<int>(object.height);
    bomb_zones.push_back(rect);
}

void GameView::render() {
    auto it = players.find(local_id);
    if (it != players.end() && it->second) {
        float px = it->second->get_x();
        float py = it->second->get_y();
        camera.center_on(px, py);
    }

    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.Clear();

    renderer.Copy(*texture_manager.get_texture(terrains[terrain]),
                  SDL2pp::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    for (const auto& zone: bomb_zones) {
        float screenX = zone.x - camera.get_x();
        float screenY = zone.y - camera.get_y();
        SDL2pp::Rect dst_rect = {static_cast<int>(screenX), static_cast<int>(screenY), int(zone.w),
                                 int(zone.h)};

        SDL_Rect src_rect = {0, 0, int(zone.w), int(zone.h)};

        renderer.Copy(*texture_manager.get_texture("bomb_zone"), src_rect, dst_rect);
    }

    for (auto& obstacle: obstacles) obstacle.draw(camera);

    for (auto& drop: drops) drop.draw(camera);

    if (bomb_view.is_exploding()) {
        bomb_view.drawExplosion(renderer, camera);
        bomb_view.reset();
    } else if (bomb_view.is_active() || bomb_view.is_dropped()) {
        bomb_view.draw(renderer, camera);
    }

    for (auto& bullet: bullets) bullet.draw(renderer, camera);

    for (auto& [id, player]: players) player->draw(renderer, camera);

    if (fov_view.is_visible() && players[local_id]) {
        fov_view.draw(players[local_id]->get_angle());
    }

    hud_view.draw(bomb_view.is_active());

    shop_view.render();

    stats_view.render();
    if (show_waiting_lobby)
        stats_view.render_waiting_lobby();

    if (game_ended)
        stats_view.render_winner_banner(winner);


    render_cursor();

    renderer.Present();
}

void GameView::frame_sync() {
    static uint32_t last_frame = SDL_GetTicks();
    uint32_t now = SDL_GetTicks();
    uint32_t elapsed = now - last_frame;

    if (elapsed < frame_delay)
        SDL_Delay(frame_delay - elapsed);

    last_frame = SDL_GetTicks();
}

void GameView::render_cursor() {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int scaled_width = POINTER_WIDTH / 2;
    int scaled_height = POINTER_HEIGHT / 2;

    renderer.Copy(*texture_manager.get_texture("pointer"),
                  SDL2pp::Rect(0, 0, POINTER_WIDTH, POINTER_HEIGHT),
                  SDL2pp::Rect(mouseX - scaled_width / 2, mouseY - scaled_height / 2, scaled_width,
                               scaled_height));
}


void GameView::handle_attack() {
    if (is_alive) {
        WeaponModel current_weapon = players[local_id]->get_current_weapon();
        if (current_weapon == WeaponModel::KNIFE) {
            players[local_id]->start_knife_animation();
        } else {
            players[local_id]->start_kickback();
        }
    }
}
std::vector<float> GameView::player_position() {
    auto it = players.find(local_id);
    if (it != players.end() && it->second) {
        float x = it->second->get_x();
        float y = it->second->get_y();
        return {x, y};
    }
    return {};
}

void GameView::show_end_banner(WinnerTeamType winner) {
    stats_view.set_visible(false);
    this->winner = winner;
    game_ended = true;
}

bool GameView::round_active() const {
    return !game_ended && !show_waiting_lobby && !stats_view.is_visible() &&
           !shop_view.is_visible() && is_alive;
}
