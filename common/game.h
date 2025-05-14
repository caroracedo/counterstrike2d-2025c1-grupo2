#ifndef GAME_H
#define GAME_H

#include <utility>
#include <vector>

#include "action_DTO.h"

class Game {
private:
    std::vector<uint8_t> position;

public:
    /*
     * Constructor.
     **/
    Game(): position(2, 0) {}  // Vector con dos ceros: x = 0, y = 0

    /* Mover */
    bool move(Direction direction) {
        switch (direction) {
            case Direction::UP:
                position[1] += 1;
                break;
            case Direction::DOWN:
                position[1] -= 1;
                break;
            case Direction::LEFT:
                position[0] -= 1;
                break;
            case Direction::RIGHT:
                position[0] += 1;
                break;
            default:
                return false;
        }
        return true;
    }

    /* Getters */
    std::vector<uint8_t> get_position() const { return position; }
};

#endif  // GAME_H

/*
jugador -> juego.me_quiero_mover_para(direccion)
juego.pop(eventos) -> juego.actualizar_logica() -> jugadores.avisar_actualizacion()
jugador.recibir_actualizacion()
jugador.graficar()
*/

/*
jugador -> le dice al juego lo que quiere hacer -> juego.mover(tal direccion)
juego -> lo hace y le envia a TODOS los jugadores lo que hizo
queue : cada elemento es <id_jugador, evento(movimiento)>
loop:
    id, posicion = queue.pop()
    juego.jugar_jugador(id, posicion)
    broadcast
*/

/*
client_handler -> hilo receptor, hilo enviador
receptor tiene la queue con los eventos y el clienthandler hace "pop" de los eventos y se los manda
al juego junto con un id del jugador receptor recibe el mensaje por socket con DTO queue
client_handler procesa, actualiza la lógica
*/
