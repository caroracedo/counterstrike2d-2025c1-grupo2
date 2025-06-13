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
        shop_view(renderer),
        stats_view(renderer) {
    SDL_ShowCursor(SDL_DISABLE);
}


void GameView::update(const ActionDTO& action) {
    if (action.type == ActionType::SHOP) {
        bomb_view.reset();
        shop_view.set_visible(true);
        stats_view.set_visible(false);
    }

    if (action.type == ActionType::STATS) {
        stats_view.update(action.stats, types);
        stats_view.set_visible(true);
    }

    if (action.type != ActionType::UPDATE)
        return;

    shop_view.set_visible(false);
    stats_view.set_visible(false);
    obstacles.clear();
    bullets.clear();
    bomb_zones.clear();

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
            if (object.bomb_countdown == 0) {
                bomb_view.explode();
            } else {
                bomb_view.activate_bomb();
            }
            hud_view.update_timer(object);
            bomb_view.update(object.position[0], object.position[1]);

        } else if (object.type == ObjectType::BOMBZONE) {
            SDL_Rect rect;
            rect.x = static_cast<int>(object.position[0]);
            rect.y = static_cast<int>(object.position[1]);
            rect.w = static_cast<int>(object.width);
            rect.h = static_cast<int>(object.height);
            bomb_zones.push_back(rect);
        }
    }

    for (auto it = players.begin(); it != players.end();) {
        if (players_in_game.find(it->first) == players_in_game.end()) {
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

    players.try_emplace(id, std::make_unique<PlayerView>(texture_manager, id));

    legs.try_emplace(id, std::make_unique<LegsView>(*texture_manager.get_texture("legs"), 100));

    guns.try_emplace(id, std::make_unique<GunView>(renderer));

    players[id]->update_styles(object.player_type, object.weapon_model);

    guns[id]->change_gun(object.weapon_model);

    types.try_emplace(id, object.player_type);

    if (players[id]->update_position(x, y)) {
        legs[id]->update(x, y);
        guns[id]->update(x, y);
    }

    if (id == local_id) {
        hud_view.update(object);
    }
}

void GameView::update_bullets(const ObjectDTO& object) {
    BulletView bullet(object.position[0], object.position[1]);
    // bullet.angle = object.angle; // Asumiendo que el DTO tiene un campo de ángulo
    bullets.push_back(bullet);
}

void GameView::update_obstacles(const ObjectDTO& object) {
    ObstacleView obs(renderer, texture_manager);

    obs.update(object.position[0], object.position[1], object.width, object.height,
               object.obstacle_type);

    obstacles.push_back(obs);
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

    renderer.Copy(*texture_manager.get_texture("background"),
                  SDL2pp::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    for (const auto& zone: bomb_zones) {
        float screenX = zone.x - camera.get_x();
        float screenY = zone.y - camera.get_y();
        SDL2pp::Rect dst_rect = {static_cast<int>(screenX) + (OBSTACLE_WIDTH / 2),
                                 static_cast<int>(screenY) + (OBSTACLE_HEIGHT / 2), int(zone.w),
                                 int(zone.h)};

        SDL_Rect src_rect = {0, 0, int(zone.w), int(zone.h)};

        renderer.Copy(*texture_manager.get_texture("bomb_zone"), src_rect, dst_rect);
    }

    for (auto& bullet: bullets) bullet.draw(renderer, camera);

    for (auto& [id, leg]: legs) leg->draw(renderer, camera);

    for (auto& [id, player]: players) player->draw(renderer, camera);

    for (auto& [id, gun]: guns) {
        gun->draw(renderer, camera);
    }

    for (auto& obstacle: obstacles) obstacle.draw(camera);

    if (bomb_view.is_exploding()) {
        bomb_view.drawExplosion(renderer, camera);
        bomb_view.reset();
    } else if (bomb_view.is_active()) {
        bomb_view.draw(renderer, camera);
    }

    hud_view.draw(bomb_view.is_active());

    shop_view.render();

    stats_view.render();

    render_cursor();

    renderer.Present();
}

// void GameView::update_graphics(const ActionDTO& action){
//     update(action);
//     render();
// }

void GameView::frame_sync() {
    uint32_t now = SDL_GetTicks();
    uint32_t elapsed = now - last_frame_time;

    if (elapsed < frame_delay)
        SDL_Delay(frame_delay - elapsed);

    last_frame_time = SDL_GetTicks();
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
