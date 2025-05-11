#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <utility>

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>

// #include "client_graphic_parser.h"
#include "client_protocol.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const int PLAYER_SIZE = 50;
const int PLAYER_SPEED = 5;

class Client {
private:
    ClientProtocol protocol;
    // GraphicParser graphic_parser;

public:
    /*
     * Constructor.
     **/
    explicit Client(const char* hostname, const char* servname):
            protocol(std::move(Socket(hostname, servname))) {}

    /* Iniciar la comunicación */
    void initiate_communication() {
        while (true) {
            try {
                // // // Initialize SDL library
                // SDL_Init(SDL_INIT_VIDEO);

                // // // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
                // // Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                // 640, 480, SDL_WINDOW_RESIZABLE); SDL_Window* window = SDL_CreateWindow("prueba
                // ventana", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                // SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

                // // // Create accelerated video renderer with default driver
                // // Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
                // SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                // SDL_RENDERER_ACCELERATED);

                // // prueba de movimiento de un rectángulo rojo sobre la pantalla
                // SDL_Rect player = {SCREEN_WIDTH / 2 - PLAYER_SIZE / 2, SCREEN_HEIGHT / 2 -
                // PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_SIZE};

                // bool running = true;

                // SDL_Event event;

                // while (running) {
                //     // Acá nos llega la acción
                //     while (SDL_PollEvent(&event)) {
                //         if (event.type == SDL_QUIT)
                //             running = false;
                //         if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                //             running = false;
                //     }


                //     // La parseamos
                //     ActionDTO action_request;
                //     action_request.type = action_requestType::MOVE;
                //     const Uint8* keystates = SDL_GetKeyboardState(NULL);
                //     if (keystates[SDL_SCANCODE_W])
                //         // player.y -= PLAYER_SPEED;
                //         action_request.direction = Direction::UP;
                //     if (keystates[SDL_SCANCODE_S])
                //         // player.y += PLAYER_SPEED;
                //         action_request.direction = Direction::DOWN;
                //     if (keystates[SDL_SCANCODE_A])
                //         // player.x -= PLAYER_SPEED;
                //         action_request.direction = Direction::LEFT;
                //     if (keystates[SDL_SCANCODE_D])
                //         // player.x += PLAYER_SPEED;
                //         action_request.direction = Direction::RIGHT;

                //     // Enviamos el movimiento que queremos hacer
                //     protocol.serialize_and_send_action(action_request);

                //     // Recibo respuesta (posicion nueva) como DTO
                //     ActionDTO action_update = protocol.receive_and_deserialize_move();

                //     // Actualizamos la posición
                //     if (action_update.type == ActionType::MOVE)
                //         player.x = action_update.position[0];
                //         player.y = action_update.position[1];


                //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                //     // // Clear screen
                //     // renderer.Clear();
                //     SDL_RenderClear(renderer);
                //     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                //     SDL_RenderFillRect(renderer, &player);

                //     // // Show rendered frame
                //     // renderer.Present();
                //     SDL_RenderPresent(renderer);

                //     // // 5 second delay
                //     // SDL_Delay(5000);
                //     SDL_Delay(16);
                // }
                // // // Here all resources are automatically released and library deinitialized
                // SDL_DestroyRenderer(renderer);
                // SDL_DestroyWindow(window);
                // SDL_Quit();

            } catch (...) {
                break;
            }
        }
        // protocol.close();
    }
};

#endif  // CLIENT_H
