#include "client_protocol.h"

#include <cstring>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "../common/constants.h"

explicit ClientProtocol::ClientProtocol(Socket& skt): skt(skt) {}

/*
************************************* ENVIO DE DATOS ************************************
** FORMATO: opcode (1 byte) + size (2 bytes - big-endian) + data (size bytes)          **
**     - opcode: el código de operación que indica la acción a realizar                **
**     - size: el tamaño del mensaje a enviar, en bytes                                **
**     - data: el mensaje a enviar, que puede ser un string o un vector de bytes       **
**         - En caso de no haber datos a enviar, se envía solo el opcode y el tamaño 0 **
*****************************************************************************************
*/

bool ClientProtocol::serialize_and_send(const uint8_t opcode) {
    /*
     * Envía al Server el comando (1 byte) y el largo del mensaje (0) en big-endian (2 bytes)
     */
    uint16_t length = htons(static_cast<uint16_t>(0));

    return skt_manager.send_byte(skt, opcode) && skt_manager.send_two_bytes(skt, length);
}

bool ClientProtocol::serialize_and_send(const uint8_t opcode, const std::vector<uint8_t>& data) {
    /*
     * Envía al Server el comando (1 byte), el largo del mensaje (2 bytes) en big-endian, y el
     * mensaje (size bytes)
     */
    uint16_t length = htons(static_cast<uint16_t>(data.size()));

    return (skt_manager.send_byte(skt, opcode) && skt_manager.send_two_bytes(skt, length) &&
            skt_manager.send_bytes(skt, data));
}

bool ClientProtocol::serialize_and_send(const uint8_t opcode, const std::string& data) {
    /*
     * Envía al Server el comando (1 byte), el largo del mensaje (2 bytes) en big-endian, y el
     * mensaje (size bytes)
     */

    std::vector<uint8_t> data_vector(data.begin(), data.end());
    data_vector.push_back('\0');

    return serialize_and_send(opcode, data_vector);
}

bool ClientProtocol::serialize_and_send_action(const MainMenuDTO& main_menu) {
    /*
     * Envía al Server el comando seleccionado por el Client, pudiendo ser este:
     * Create, Join, o List
     */
    switch (main_menu.option) {
        uint8_t opcode = static_cast<uint8_t>(main_menu.option);
        case Option::CREATE:
            return serialize_and_send(opcode, main_menu.game_name);
        case Option::JOIN:
            return serialize_and_send(opcode, main_menu.game_name);
        case Option::LIST:
            return serialize_and_send(opcode);
        default:
            return false;
    }
}

bool ClientProtocol::serialize_and_send_action(const ActionDTO& action) {
    /**
     * Envía al Server la acción seleccionada por el Client, pudiendo ser esta:
     *  - Moverse en una dirección
     */
    switch (action.type) {
        uint8_t opcode = static_cast<uint8_t>(action.type);
        case ActionType::MOVE: {
            std::vector<uint8_t> direction(static_cast<uint8_t>(action.direction));
            return serialize_and_send(opcode, direction);
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

std::string ClientProtocol::receive_and_deserialize_string() {
    /*
     * Recibe del Server el mensaje con los nombres de las partidas actuales
     * y lo deserializa a un string.
     */
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size)) {
        return {};
    }

    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data)) {
        return {};
    }

    std::string text(data.begin(), data.end());
    return text;
}

std::vector<uint8_t> ClientProtocol::receive_and_deserialize_vector() {
    uint16_t size;
    if (!skt_manager.receive_two_bytes(skt, size)) {
        return {};
    }

    std::vector<uint8_t> data(size);
    if (!skt_manager.receive_bytes(skt, data)) {
        return {};
    }

    return data;
}

std::string ClientProtocol::receive_and_deserialize_list() {
    /*
     * Recibe del Server el mensaje con los nombres de las partidas actuales
     * y lo deserializa a un string.
     */
    return receive_and_deserialize_string();
}

ActionDTO ClientProtocol::receive_and_deserialize_updated_position() {
    /*
     * Recibe del Server la posición actualizada del jugador y la deserializa
     * a un vector de bytes.
     */
    std::vector<uint8_t> data = receive_and_deserialize_vector();
    if (data.empty()) {
        return {};
    }

    return {static_cast<ActionType>(data[0]), std::vector<uint8_t>(data.begin() + 1, data.end())};
}

/**************************************** CIERRE ***************************************/

void ClientProtocol::close() { skt_manager.close(skt); }
