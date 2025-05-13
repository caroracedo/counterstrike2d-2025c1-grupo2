#include "server_protocol.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "../common/constants.h"

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
MainMenuDTO ServerProtocol::receive_and_deserialize_main_menu_action() {
    /*
     * Recibe comando del Main Menu (Create, Join, List) enviada por el Client
     * En caso de ser un comando, devuelve un MainMenuDTO con campos necesarios completos
     * Caso contrario, devuelve un MainMenuDTO vacío (option=UNKNOWN)
     */
    uint8_t opcode;
    uint16_t size;
    if (!skt_manager.receive_byte(skt, opcode) || !skt_manager.receive_two_bytes(skt, size)) {
        return {};
    }
    switch (static_cast<Option>(opcode)) {
        case Option::CREATE:
            return receive_and_deserialize_string(Option::CREATE, size);
        case Option::JOIN:
            return receive_and_deserialize_string(Option::JOIN, size);
        case Option::LIST:
            return MainMenuDTO(Option::LIST);
        default:
            return {};
    }
}

MainMenuDTO ServerProtocol::receive_and_deserialize_string(const Option& action_type,
                                                           const uint16_t& size) {
    /*
     * Recibe el mensaje (string) de largo size enviado por el Client
     * Usado cuando el comando es Option::CREATE o Option::JOIN
     */

    std::vector<uint8_t> data(size);
    if (size == 0 || !skt_manager.receive_bytes(skt, data)) {
        return {};
    }

    return {action_type, std::string(data.begin(), data.end())};
}

ActionDTO ServerProtocol::receive_and_deserialize_action() {
    /*
     * Recibe el opcode de la acción enviada por el cliente y el largo del mensaje
     * y lo deserializa a un ActionDTO. Si no reconoce la acción,
     * devuelve un ActionDTO vacío
     */
    uint8_t type;
    uint16_t size;
    if (!skt_manager.receive_byte(skt, type) || !skt_manager.receive_two_bytes(skt, size)) {
        return {};
    }

    switch (static_cast<ActionType>(type)) {
        case ActionType::MOVE:
            return receive_and_deserialize_move(ntohs(size));

        default:
            return {};
    }
}

ActionDTO ServerProtocol::receive_and_deserialize_move(const uint16_t& size) {
    /*
     * Recibe el opcode de la dirección y lo deserializa a un ActionDTO.
     */

    uint8_t direction;
    if (!skt_manager.receive_byte(skt, direction)) {
        return {};
    }

    switch (static_cast<Direction>(direction)) {
        case (Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT):
            return {ActionType::MOVE, static_cast<Direction>(direction)};
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

bool ServerProtocol::serialize_and_send_games_names(const std::vector<std::string>& names) {
    /*
     * Recibe un vector con los nombres de las partidas actuales y se las envía al Client
     * con el formato adecuado.
     */
    std::ostringstream oss;
    oss << "Partidas:";

    for (const std::string& name: names) {
        oss << "\n - " << name;
    }
    oss << "\n";
    std::string matches = oss.str();

    // Convertir el string matches a un vector de uint8_t
    std::vector<uint8_t> data(matches.begin(), matches.end());

    // Serializar el tamaño del vector (2 bytes en formato big-endian)
    uint16_t size = htons(static_cast<uint16_t>(data.size()));

    // Enviar primero el tamaño y luego el contenido del vector
    return skt_manager.send_two_bytes(skt, size) && skt_manager.send_bytes(skt, data);
}


bool ServerProtocol::serialize_and_send_updated_position(ActionDTO action_dto) {
    /*
     * Envía al Client la posición actualizada del jugador
     */

    // vector con el opcode de la acción y la posicion
    std::vector<uint8_t> data;
    data.push_back(static_cast<uint8_t>(action_dto.type));
    data.insert(data.end(), action_dto.position.begin(), action_dto.position.end());

    uint16_t size = htons(static_cast<uint16_t>(data.size()));

    return skt_manager.send_two_bytes(skt, size) && skt_manager.send_bytes(skt, data);
}


/**************************************** CIERRE ***************************************/

void ServerProtocol::close() { skt_manager.close(skt); }
