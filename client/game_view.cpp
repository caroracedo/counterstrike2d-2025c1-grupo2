#include "game_view.h"

#include <SDL2/SDL.h>
#include <unordered_set>


// de esto voy a hacer un refactor para que no se carguen las texturas en el constructor
// se haria una clase mas prolija.
GameView::GameView():  
    sdl(SDL_INIT_VIDEO),
    window("CS2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
    renderer(window, -1, SDL_RENDERER_ACCELERATED),
    terrorist_sprites(renderer, Surface(SDL_LoadBMP("../assets/gfx/player/t1.bmp"))),
    counterterrorist_srpites(renderer, Surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp"))),
    legs_sprites(renderer, Surface(SDL_LoadBMP("../primer_fila_sin_padding.bmp"))),
    background (renderer, Surface(SDL_LoadBMP("../dustroof.bmp"))),
    box_texture(renderer, Surface(SDL_LoadBMP("../cuadro_fila5_columna3.bmp"))),
    box_texture2(renderer, Surface(SDL_LoadBMP("../recorte_fila5-6_columna4-5.bmp"))),
    hud_numbres(renderer, Surface(SDL_LoadBMP("../assets/gfx/hud_nums.bmp")).SetColorKey(true, 0)),
    gun_texture(renderer, Surface(SDL_LoadBMP("../assets/gfx/weapons/deagle.bmp"))), 
    bomb_texture(renderer,Surface(SDL_LoadBMP("../assets/gfx/weapons/bomb.bmp"))),
    explotion_sprites(renderer,Surface(SDL_LoadBMP("../explosion.bmp"))),
    camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2048, 2048),
    hud_view(hud_numbres, renderer),
    bomb_view(bomb_texture, explotion_sprites)
    {}


void GameView::update(const ActionDTO& action) {
   
    if (action.type != ActionType::UPDATE)
        return;

    obstacles.clear();
    bullets.clear();
    std::unordered_set<uint8_t> players_in_game;
    for(const auto& object : action.objects) {

        if (object.type == ObjectType::PLAYER){
            std::cout << "Player ID: " << object.id << std::endl;
            update_player(object);
            players_in_game.insert(object.id);
        }
       
        // se encapsula en una clase
        else if (object.type == ObjectType::OBSTACLE) 
            update_obstacles(object);

        else if (object.type == ObjectType::BULLET)
            update_bullets(object);
        
        else if (object.type == ObjectType::BOMB) {
            if (object.bomb_countdown <= 0) {
                bomb_view.explode();
            } else {
                bomb_view.activate_bomb();
            }
            hud_view.update_timer(object);
            bomb_view.update(object.position[0], object.position[1]);
        }
    }

    for (auto it = players.begin(); it != players.end(); ) {
        if (players_in_game.find(it->first) == players_in_game.end()) {
            it = players.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = legs.begin(); it != legs.end(); ) {
        if (players_in_game.find(it->first) == players_in_game.end()) {
            it = legs.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = guns.begin(); it != guns.end(); ) {
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

    
    if (players.find(id) == players.end()) {
        players[id] = std::make_unique<PlayerView>(
            object.player_type == PlayerType::TERRORIST
                ? terrorist_sprites
                : counterterrorist_srpites
        );
    }
    
    
    if (legs.find(id) == legs.end()) {
        legs[id] = std::make_unique<LegsView>(legs_sprites,std::vector<SDL2pp::Rect>{
            Rect(0, 0, 32, 32),
            Rect(32, 0, 32, 32),
            Rect(64, 0, 32, 32)
        },
        100);
    }

    if (guns.find(id) == guns.end()) {
        guns[id] = std::make_unique<GunView>(gun_texture);
    }
    
    if (x - last_px  || y - last_py  == 5|| last_px == -1 || last_py == -1) {
        last_px = x;
        last_py = y;
        players[id]->update_position(x, y);
        legs[id]->update_position(x, y);
        legs[id]->update_animation();
        guns[id]->update(x,y);
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
    obs.use_texture2 = (object.width == 64 && object.height == 64); // ejemplo de uso de textura2
    obstacles.push_back(obs);
}


void GameView::render() {
    auto it = players.find(local_id);
    if (it != players.end() && it->second) {
        float px = it->second->get_x();
        float py = it->second->get_y();
        camera.center_on(px + PLAYER_WIDTH / 2, py + PLAYER_HEIGHT / 2);  
    }

    renderer.SetDrawColor(255, 255, 255, 255);
    renderer.Clear();

    // Rect view = camera.get_viewport();

    renderer.Copy(background, Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));      

    
    for (auto& bullet : bullets)
    bullet.draw(renderer, camera);
    
    for (auto& [id, leg] : legs)
        leg->draw(renderer, camera);

    for (auto& [id, player] : players)
        player->draw(renderer, camera);

    for (auto& [id, gun] : guns) {
        // gun->update_angle(players[id]->get_angle());
        gun->draw(renderer, camera);
    }

    for (const auto& obs : obstacles) {
        SDL_Rect dst_rect = { int(obs.x) - camera.get_x() + (OBSTACLE_WIDTH / 2), int(obs.y) - camera.get_y() + (OBSTACLE_HEIGHT / 2), int(obs.w), int(obs.h) };
        SDL_Rect src_rect = { 0, 0, int(obs.w), int(obs.h) }; // O el tamaño real de la textura
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
    
    renderer.Present();
    
}

// void GameView::update_graphics(const ActionDTO& action){
//     update(action);
//     render();
// }

void GameView::frame_sync() {
    Uint32 now = SDL_GetTicks();
    Uint32 elapsed = now - last_frame_time;

    if (elapsed < frame_delay)
        SDL_Delay(frame_delay - elapsed);

    last_frame_time = SDL_GetTicks();
}
