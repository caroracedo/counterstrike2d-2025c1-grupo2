#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>

// #include <SDL2/SDL.h>
// #include <SDL2pp/SDL2pp.hh>

// #include "client_graphic_parser.h"
#include "client_protocol.h"

// using namespace SDL2pp;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const int PLAYER_SIZE = 50;
const int PLAYER_SPEED = 5;

class Client {
private:
    Socket client_socket;
    ClientProtocol protocol;
    // GraphicParser graphic_parser;

public:
    /*
     * Constructor.
     **/
    explicit Client(const char* hostname, const char* servname):
            client_socket(hostname, servname), protocol(this->client_socket) {}

    /* Iniciar la comunicación */
    void initiate_communication() {
        try {
            // // // Initialize SDL library
            // SDL sdl(SDL_INIT_VIDEO);

            // // // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
            // // Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            // 640, 480, SDL_WINDOW_RESIZABLE); Window window("prueba ventana",
            // SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
            // SDL_WINDOW_SHOWN);

            // // // Create accelerated video renderer with default driver
            // // Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
            // Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

            // // prueba de movimiento de un rectángulo rojo sobre la pantalla
            // Rect player = {SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 - PLAYER_SIZE /
            // 2, PLAYER_SIZE, PLAYER_SIZE};

            // SDL_Event event;

            // while (true) {
            //     // Acá nos llega la acción
            //     while (SDL_PollEvent(&event)) {
            //         if (event.type == SDL_QUIT)
            //             break;
            //         if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            //             break;
            //     }

            //     // La parseamos
            //     Direction direction;
            //     const Uint8 *keystates = SDL_GetKeyboardState(NULL);
            //     if (keystates[SDL_SCANCODE_W])
            //         // player.y -= PLAYER_SPEED;
            //         direction = Direction::UP;
            //     if (keystates[SDL_SCANCODE_S])
            //         // player.y += PLAYER_SPEED;
            //         direction = Direction::DOWN;
            //     if (keystates[SDL_SCANCODE_A])
            //         // player.x -= PLAYER_SPEED;
            //         direction = Direction::LEFT;
            //     if (keystates[SDL_SCANCODE_D])
            //         // player.x += PLAYER_SPEED;
            //         direction = Direction::RIGHT;

            //     // Enviamos el movimiento que queremos hacer
            //     if (!protocol.serialize_and_send_action({ActionType::MOVE, direction}))
            //         break;

            //     // Recibo respuesta (posicion nueva) como DTO
            //     ActionDTO action_update = protocol.receive_and_deserialize_move();

            //     // Actualizamos la posición
            //     if (action_update.type == ActionType::MOVE) {
            //         player.x = action_update.position[0];
            //         player.y = action_update.position[1];
            //     }

            //     renderer.SetDrawColor(255, 255, 255, 255);
            //     // // Clear screen
            //     // renderer.Clear();
            //     renderer.Clear();

            //     renderer.SetDrawColor(255, 0, 0, 255);
            //     renderer.FillRect(player);

            //     // // Show rendered frame
            //     // renderer.Present();
            //     renderer.Present();

            //     // // 5 second delay
            //     // SDL_Delay(5000);
            //     SDL_Delay(16);

            //     // // Here all resources are automatically released and library deinitialized
            // }
        } catch (...) {}  // Por el momento...

        client_socket.shutdown(2);  // Cierra lectura y escritura
        client_socket.close();
    }
};

#endif  // CLIENT_H
