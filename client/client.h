#ifndef CLIENT_H
#define CLIENT_H

#include <utility>

#include "client_protocol.h"
#include "../common/action_DTO.h"
#include "input_handler.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Surface.hh>
#include <cmath>


using namespace SDL2pp;

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;

public:
    explicit Client(const char* hostname, const char* servname):
            client_socket(hostname, servname), protocol(this->client_socket) {}

    void initiate_communication() {
        // Inicialización
        SDL sdl(SDL_INIT_VIDEO);
        Window window("Jugador compuesto", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        
        // Cargar recursos
        Surface cuerpo_surface(SDL_LoadBMP("../assets/gfx/player/ct1.bmp")); // TODO: Preguntar

        Texture cuerpo(renderer, cuerpo_surface);
        
        Surface piernas_surface(SDL_LoadBMP("../assets/gfx/player/legs.bmp"));
        Texture piernas(renderer, piernas_surface);

        Surface mira_surface(SDL_LoadBMP("../assets/gfx/pointer.bmp"));
        mira_surface.SetColorKey(true, SDL_MapRGB(mira_surface.Get()->format, 255, 0, 255));
        Texture mira(renderer, mira_surface);

        InputHandler ih(renderer, cuerpo, piernas);

        // LoopClient
        while (true) {
            try {
                ActionDTO action = ih.receive_and_parse_action();
                if (action.type == ActionType::QUIT)
                    break;
                if (action.type == ActionType::UNKNOWN) {
                    ih.update_graphics(action);
                    continue;
                }
                if (!protocol.serialize_and_send_action(action))
                    break;
                
                // Movimiento
                
                ActionDTO action_update = protocol.receive_and_deserialize_updated_position();
        
                if (action_update.type == ActionType::UNKNOWN) {
                    continue;
                }  
                if (!ih.update_graphics(action_update))
                    break;
            } catch (...) {}  // Por el momento...
        }

        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();
    }
};

#endif  // CLIENT_H
