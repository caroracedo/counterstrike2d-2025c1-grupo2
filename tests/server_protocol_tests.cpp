#include <chrono>
#include <thread>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "common/action_DTO.h"
#include "server/server_protocol.h"

#include "tests_constants.h"

// Para ver más detalles del protocolo observar la documentación técnica
// Igual esto va a cambiar con el pre-lobby y el lobby
TEST(SERVER_PROTOCOL, LOBBY_CREATE) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME * 0));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Envío de INFORMATION: partidas y mapas
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::INFORMATION, {MATCH_1, MATCH_2}, {MAP_1, MAP_2}}))
            << "Sending information should succeed.";

    // Recepción de CREATE: partida y mapa deseados
    ActionDTO create = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::CREATE;
    std::string expected_match = MATCH_3;
    std::string expected_map = MAP_1;
    PlayerType expected_player_type = PlayerType::TERRORIST;
    EXPECT_EQ(create.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(create.match, expected_match) << "Received match do not match expected match.";
    EXPECT_EQ(create.map, expected_map) << "Received map do not match expected map.";
    EXPECT_EQ(create.player_type, expected_player_type)
            << "Received player type do not match expected player type.";

    // Envío de CONFIGURATION: tipo de terreno y id
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::CONFIGURATION, MAP_1_TERRAIN_TYPE, ID_1}))
            << "Sending configuration should succeed.";

    protocol.kill();
}

TEST(SERVER_PROTOCOL, LOBBY_JOIN) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME * 2));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Envío de INFORMATION: partidas y mapas
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::INFORMATION, {MATCH_1, MATCH_2, MATCH_3}, {MAP_1, MAP_2}}))
            << "Sending information should succeed.";

    // Recepción de JOIN: partida deseada
    ActionDTO join = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::JOIN;
    std::string expected_match = MATCH_3;
    PlayerType expected_player_type = PlayerType::COUNTERTERRORIST;
    EXPECT_EQ(join.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(join.match, expected_match) << "Received match do not match expected match.";
    EXPECT_EQ(join.player_type, expected_player_type)
            << "Received player type do not match expected player type.";

    // Envío de CONFIGURATION: tipo de terreno y id
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::CONFIGURATION, MAP_1_TERRAIN_TYPE, ID_1}))
            << "Sending configuration should succeed.";

    protocol.kill();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
