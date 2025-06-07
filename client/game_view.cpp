#include "game_view.h"

#include <unordered_set>
#include <vector>

#include <SDL2/SDL.h>

// de esto voy a hacer un refactor para que no se carguen las texturas en el constructor
// se haria una clase mas prolija.
GameView::GameView():
        sdl(SDL_INIT_VIDEO),
        window("CS2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
               SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
        renderer(window, -1, SDL_RENDERER_ACCELERATED),
        terrorist_sprites(renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/player/t1.bmp"))),
        counterterrorist_srpites(renderer,
                                 SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp"))),
        legs_sprites(renderer, SDL2pp::Surface(SDL_LoadBMP("../primer_fila_sin_padding.bmp"))),
        background(renderer, SDL2pp::Surface(SDL_LoadBMP("../dustroof.bmp"))),
        box_texture(renderer, SDL2pp::Surface(SDL_LoadBMP("../cuadro_fila5_columna3.bmp"))
                                      .SetBlendMode(SDL_BLENDMODE_BLEND)),
        box_texture2(renderer, SDL2pp::Surface(SDL_LoadBMP("../recorte_fila5-6_columna4-5.bmp"))
                                       .SetBlendMode(SDL_BLENDMODE_BLEND)),
        hud_numbres(
                renderer,
                SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/hud_nums.bmp")).SetColorKey(true, 0)),
        gun_texture(renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/deagle.bmp"))),
        bomb_texture(renderer, SDL2pp::Surface(SDL_LoadBMP("../assets/gfx/weapons/bomb.bmp"))),
        explotion_sprites(renderer, SDL2pp::Surface(SDL_LoadBMP("../explosion.bmp"))),
        bomb_zone(renderer, SDL2pp::Surface(SDL_LoadBMP("../bomb_zone.bmp"))),
        camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2048, 2048),
        hud_view(hud_numbres, renderer),
        bomb_view(bomb_texture, explotion_sprites),
        shop_view(renderer) {}


void GameView::update(const ActionDTO& action) {
    if (action.type == ActionType::SHOP) {
        std::cout << "Action Type: " << static_cast<int>(action.type) << std::endl;
        shop_view.set_visible(true);
    }

    if (action.type != ActionType::UPDATE)
        return;
    shop_view.set_visible(false);
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


    players.try_emplace(id, std::make_unique<PlayerView>(renderer));

    legs.try_emplace(
            id, std::make_unique<LegsView>(legs_sprites,
                                           std::vector<SDL2pp::Rect>{SDL2pp::Rect(0, 0, 32, 32),
                                                                     SDL2pp::Rect(32, 0, 32, 32),
                                                                     SDL2pp::Rect(64, 0, 32, 32)},
                                           100));

    guns.try_emplace(id, std::make_unique<GunView>(renderer));


    if (x - last_px || y - last_py == 5 || last_px == -1 || last_py == -1) {
        players[id]->update_position(x, y);
        players[id]->update_styles(object.player_type, object.weapon_model);
        // last_px = x;
        // last_py = y;
        legs[id]->update_position(x, y);
        legs[id]->update_animation();
        guns[id]->update(x, y);
        guns[id]->change_gun(object.weapon_model);
        // sound_manager.playFromGroup("steps",0);
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
    ObstacleView obs;
    obs.x = object.position[0];
    obs.y = object.position[1];
    obs.w = object.width;
    obs.h = object.height;
    obs.use_texture2 = (object.width == 64 && object.height == 64);  // ejemplo de uso de textura2
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

    renderer.Copy(background, SDL2pp::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    for (const auto& zone: bomb_zones) {
        float screenX = zone.x - camera.get_x();
        float screenY = zone.y - camera.get_y();
        SDL2pp::Rect dst_rect = {static_cast<int>(screenX) + (OBSTACLE_WIDTH / 2),
                                 static_cast<int>(screenY) + (OBSTACLE_HEIGHT / 2), int(zone.w),
                                 int(zone.h)};

        SDL_Rect src_rect = {0, 0, int(zone.w), int(zone.h)};
        box_texture2.SetAlphaMod(255);  // 50% transparent
        box_texture.SetAlphaMod(255);

        renderer.Copy(bomb_zone, src_rect, dst_rect);
    }

    for (auto& bullet: bullets) bullet.draw(renderer, camera);

    for (auto& [id, leg]: legs) leg->draw(renderer, camera);

    for (auto& [id, player]: players) player->draw(renderer, camera);

    for (auto& [id, gun]: guns) {
        // gun->update_angle(players[id]->get_angle());
        gun->draw(renderer, camera);
    }

    for (const auto& obs: obstacles) {
        float screenX = obs.x - camera.get_x();
        float screenY = obs.y - camera.get_y();
        SDL2pp::Rect dst_rect = {static_cast<int>(screenX) + (OBSTACLE_WIDTH / 2),
                                 static_cast<int>(screenY) + (OBSTACLE_HEIGHT / 2), int(obs.w),
                                 int(obs.h)};

        SDL_Rect src_rect = {0, 0, int(obs.w), int(obs.h)};
        box_texture2.SetAlphaMod(255);  // 50% transparent
        box_texture.SetAlphaMod(255);
        if (obs.use_texture2) {
            renderer.Copy(box_texture2, src_rect, dst_rect);
        } else {
            renderer.Copy(box_texture, src_rect, dst_rect);
        }
    }
    if (bomb_view.is_exploding()) {
        bomb_view.drawExplosion(renderer, camera);
        bomb_view.reset();
    } else if (bomb_view.is_active()) {
        bomb_view.draw(renderer, camera);
    }

    hud_view.draw();

    shop_view.render();

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
