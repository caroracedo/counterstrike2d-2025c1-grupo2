#include "game_view.h"

#include <SDL2/SDL.h>

// de esto voy a hacer un refactor para que no se carguen las texturas en el constructor
// se haria una clase mas prolija.
GameView::GameView():  
    sdl(SDL_INIT_VIDEO),
    window("CS2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN),
    renderer(window, -1, SDL_RENDERER_ACCELERATED),
    body_sprites(renderer, Surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp"))),
    legs_sprites(renderer, Surface(SDL_LoadBMP("../primer_fila_sin_padding.bmp"))),
    background (renderer, Surface(SDL_LoadBMP("../dustroof.bmp"))),
    camera(SCREEN_WIDTH, SCREEN_HEIGHT, 2048, 2048),
    // gun_texture(renderer, Surface(SDL_LoadBMP("../assets/gfx/weapons/ak47.bmp"))),
    // gun_view(gun_texture) 
    box_texture(renderer, Surface(SDL_LoadBMP("../cuadro_fila5_columna3.bmp"))),
    box_texture2(renderer, Surface(SDL_LoadBMP("../recorte_fila5-6_columna4-5.bmp"))),
    hud_numbres(renderer, Surface(SDL_LoadBMP("../assets/gfx/hud_nums.bmp")).SetColorKey(true, 0)),
    hud_view(hud_numbres, renderer)
    {}


void GameView::update(const ActionDTO& action) {
   
    if (action.type != ActionType::UPDATE)
        return;

    obstacles.clear();
    bullets.clear();
    for(const auto& object : action.objects) {

        if (object.type == ObjectType::PLAYER)
            update_player(object);
       
        // se encapsula en una clase
        else if (object.type == ObjectType::OBSTACLE) 
            update_obstacles(object);

        else if (object.type == ObjectType::BULLET)
            update_bullets(object);
    }    
}

void GameView::update_player(const ObjectDTO& object) {
    uint16_t id = object.id;
    float x = object.position[0];
    float y = object.position[1];

    // Si no existe, lo creo
    
    // if (local_id == 0 ) {
    //     local_id = id;
    // }
    
    
    if (players.find(id) == players.end()) {
        players[id] = std::make_unique<PlayerView>(body_sprites);
    }
    
    
    if (legs.find(id) == legs.end()) {
        legs[id] = std::make_unique<LegsView>(legs_sprites,std::vector<SDL2pp::Rect>{
            Rect(0, 0, 32, 32),
            Rect(32, 0, 32, 32),
            Rect(64, 0, 32, 32)
        },
        100);
    }
    
    if (x - last_px  || y - last_py  == 5|| last_px == -1 || last_py == -1) {
        last_px = x;
        last_py = y;
        players[id]->update_position(x, y);
        legs[id]->update_position(x, y);
        legs[id]->update_animation();
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

    for (const auto& obs : obstacles) {
        SDL_Rect dst_rect = { int(obs.x) - camera.get_x(), int(obs.y) - camera.get_y(), int(obs.w), int(obs.h) };
        SDL_Rect src_rect = { 0, 0, int(obs.w), int(obs.h) }; // O el tamaño real de la textura
        if (obs.use_texture2) {
            renderer.Copy(box_texture2, src_rect, dst_rect);
        } else {
            renderer.Copy(box_texture, src_rect, dst_rect);
        }
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
