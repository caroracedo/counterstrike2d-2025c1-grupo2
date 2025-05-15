#include "client_protocol.h"

#include <cstring>
#include <utility>
#include <vector>

#include "../common/constants.h"

ClientProtocol::ClientProtocol(Socket& skt): skt(skt) {}

/*
************************************* ENVIO DE DATOS ************************************
** FORMATO: opcode (1 byte) + size (2 bytes - big-endian) + data (size bytes)          **
**     - opcode: el código de operación que indica la acción a realizar                **
**     - size: el tamaño del mensaje a enviar, en bytes                                **
**     - data: el mensaje a enviar, que puede ser un string o un vector de bytes       **
**         - En caso de no haber datos a enviar, se envía solo el opcode y el tamaño 0 **
*****************************************************************************************
*/

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    /**
     * Envía al Server la acción seleccionada por el Client, pudiendo ser esta:
     *  - Moverse en una dirección
     */
    uint8_t opcode = static_cast<uint8_t>(action.type);
    switch (action.type) {
        case ActionType::MOVE: {
            std::vector<uint8_t> data = {opcode, static_cast<uint8_t>(action.direction)};
            return skt_manager.send_two_bytes(skt, data.size()) &&
                   skt_manager.send_bytes(skt, data);
        }
        default:
            return false;
    }
}

/*
*********************************** RECEPCIÓN DE DATOS **********************************
** FORMATO: size (2 bytes - big-endian) + data (size bytes)                            **
**     - size: el tamaño del mensaje a recibir, en bytes                               **
**     - data: el mensaje a recibir, que puede ser un string o un vector de bytes      **
*****************************************************************************************
*/

ActionDTO ClientProtocol::receive_and_deserialize_updated_position() {
    /*
     * Recibe del Server la posición actualizada del jugador y la deserializa
     * a un vector de bytes.
     */
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size)) {
        return {};
    }
    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data)) {
        return {};
    }
    if (data.empty()) {
        return {};
    }
    return {static_cast<ActionType>(data[0]), std::vector<uint8_t>(data.begin() + 1, data.end())};
}
