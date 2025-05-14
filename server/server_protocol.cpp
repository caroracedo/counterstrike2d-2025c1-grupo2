#include "server_protocol.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

ServerProtocol::ServerProtocol(Socket& skt): skt(skt) {}

/*
*********************************** RECEPCIÓN DE DATOS **********************************
** FORMATO: opcode (1 byte) + size (2 bytes - big-endian) + data (size bytes)          **
**     - opcode: el código de operación que indica la acción a realizar                **
**     - size: el tamaño del mensaje a enviar, en bytes                                **
**     - data: el mensaje a enviar, que puede ser un string o un vector de bytes       **
**         - En caso de no haber datos a enviar, se envía solo el opcode y el tamaño 0 **
*****************************************************************************************
*/
ActionDTO ServerProtocol::receive_and_deserialize_action() {
    /*
     * Recibe el opcode de la acción enviada por el cliente y el largo del mensaje
     * y lo deserializa a un ActionDTO. Si no reconoce la acción,
     * devuelve un ActionDTO vacío
     */
    uint8_t type_byte;
    if (!skt_manager.receive_byte(skt, type_byte)) {
        return {};
    }

    ActionType type = static_cast<ActionType>(type_byte);
    switch (type) {
        case ActionType::MOVE:
            uint8_t direction_byte;
            if (!skt_manager.receive_byte(skt, direction_byte)) {
                return {};
            }
            return {type, static_cast<Direction>(direction_byte)};
        default:
            return {};
    }
}

/*
************************************* ENVIO DE DATOS ************************************
** FORMATO: size (2 bytes - big-endian) + data (size bytes)                            **
**     - size: el tamaño del mensaje a enviar, en bytes                                **
**     - data: el mensaje a enviar, que puede ser un string o un vector de bytes       **
*****************************************************************************************
*/

bool ServerProtocol::serialize_and_send_updated_position(ActionDTO action_dto) {
    /*
     * Envía al Client la posición actualizada del jugador
     */

    // vector con el opcode de la acción y la posicion
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(action_dto.type));
    data.insert(data.end(), action_dto.position.begin(), action_dto.position.end());
    return skt_manager.send_two_bytes(skt, data.size()) && skt_manager.send_bytes(skt, data);
}
