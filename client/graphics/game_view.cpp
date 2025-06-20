#include "game_view.h"

#include <unordered_set>
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
    fov_view.generarFOVTexture(diag, diag, 64, 90);
}


void GameView::update(const ActionDTO& action) {
    if (action.type == ActionType::SHOP) {
        bomb_view.reset();
        shop_view.set_visible(true);
        stats_view.set_visible(false);
        bomb_view.reset_sounds();
        sounds_played = false;
        is_first_update = false;
        is_alive = true;
    }

    if (action.type == ActionType::STATS) {
        stats_view.update(action.stats, types);
        stats_view.set_visible(true);
        stats_view.reset_sounds();
    }

    if (action.type != ActionType::UPDATE) {
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
            update_player(object);
            players_in_game.insert(object.id);
        } else if (object.type == ObjectType::OBSTACLE) {
            update_obstacles(object);
        } else if (object.type == ObjectType::BULLET) {
            update_bullets(object);
        } else if (object.type == ObjectType::BOMB) {
            bombs_in_match++;
            if (object.bomb_countdown == 0) {
                bomb_view.explode();
            } else {
                bomb_view.activate_bomb();
            }
            hud_view.update_timer(object);
            bomb_view.update(object.position[0], object.position[1], object.bomb_countdown);

        } else if (object.type == ObjectType::BOMBZONE) {
            SDL_Rect rect;
            rect.x = static_cast<int>(object.position[0]);
            rect.y = static_cast<int>(object.position[1]);
            rect.w = static_cast<int>(object.width);
            rect.h = static_cast<int>(object.height);
            bomb_zones.push_back(rect);
        } else if (object.type == ObjectType::WEAPON) {
            update_drops(object);
        }
    }

    if (bombs_in_match == 0) {
        bomb_view.reset();
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

    for (auto it = legs.begin(); it != legs.end();) {
        if (players_in_game.find(it->first) == players_in_game.end()) {
            it = legs.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = guns.begin(); it != guns.end();) {
        if (players_in_game.find(it->first) == players_in_game.end()) {
            it = guns.erase(it);
        } else {
            ++it;
        }
    }
}

void GameView::update_player(const ObjectDTO& object) {
    uint16_t id = object.id;
    float x = object.position[0];
    float y = object.position[1];

    players.try_emplace(id, std::make_unique<PlayerView>(texture_manager, sound_manager, id));

    legs.try_emplace(id, std::make_unique<LegsView>(*texture_manager.get_texture("legs"), 100));

    guns.try_emplace(id, std::make_unique<GunView>(renderer));

    players[id]->update_styles(object.player_type, object.weapon_model);

    guns[id]->change_gun(object.weapon_model);

    types.try_emplace(id, object.player_type);

    players[id]->update_angle(object.angle);
    legs[id]->update_angle(object.angle);
    guns[id]->update_angle(object.angle);

    if (players[id]->update_position(x, y, object.health)) {
        legs[id]->update(x, y);
        guns[id]->update(x, y);
        if (!is_first_update) {
            sound_manager.play("steps_" + std::to_string(rand() % 4 + 1));
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

void GameView::render() {
    auto it = players.find(local_id);
    if (it != players.end() && it->second) {
        float px = it->second->get_x();
        float py = it->second->get_y();
        camera.center_on(px, py);
    }

    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.Clear();

    // Rect view = camera.get_viewport();

    // renderer.Copy(*texture_manager.get_texture("background"),
    //               SDL2pp::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

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
    } else if (bomb_view.is_active()) {
        bomb_view.draw(renderer, camera);
    }

    for (auto& bullet: bullets) bullet.draw(renderer, camera);

    for (auto& [id, leg]: legs) leg->draw(renderer, camera);

    for (auto& [id, player]: players) player->draw(renderer, camera);

    for (auto& [id, gun]: guns) {
        gun->draw(renderer, camera);
    }

    if (fov_view.is_visible() && players[local_id]) {
        fov_view.draw(players[local_id]->get_angle());
    }

    hud_view.draw(bomb_view.is_active());

    shop_view.render();

    stats_view.render();
    if (show_pre_lobby)
        stats_view.render_pre_lobby();

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
