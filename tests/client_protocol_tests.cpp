#include <chrono>
#include <thread>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "client/client_protocol.h"
#include "common/action_DTO.h"

#include "tests_constants.h"

// Para ver más detalles del protocolo observar la documentación técnica
TEST(CLIENT_PROTOCOL, LOBBY_CREATE) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME * 1));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Recepción de INFORMATION: partidas y mapas
    ActionDTO information = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::INFORMATION;
    std::vector<std::string> expected_matches = {MATCH_1, MATCH_2};
    std::vector<std::string> expected_maps = {MAP_1, MAP_2};
    EXPECT_EQ(information.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(information.matches, expected_matches)
            << "Received matches do not match expected matches.";
    EXPECT_EQ(information.maps, expected_maps) << "Received maps do not match expected maps.";

    // Envío de CREATE: partida y mapa deseados
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::CREATE, MATCH_3, MAP_1, PlayerType::TERRORIST}))
            << "Sending match and map should succeed.";

    // Recepción de CONFIGURATION: tipo de terreno y id
    ActionDTO configuration = protocol.receive_and_deserialize_action();
    expected_type = ActionType::CONFIGURATION;
    TerrainType expected_terrain_type = MAP_1_TERRAIN_TYPE;
    uint16_t expected_id = ID_1;
    EXPECT_EQ(configuration.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(configuration.terrain_type, expected_terrain_type)
            << "Received terrain type does not match expected terrain.";
    EXPECT_EQ(configuration.id, expected_id) << "Received ID does not match expected ID.";

    protocol.kill();
}

TEST(CLIENT_PROTOCOL, LOBBY_JOIN) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME * 3));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Recepción de INFORMATION: partidas y mapas
    ActionDTO information = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::INFORMATION;
    std::vector<std::string> expected_matches = {MATCH_1, MATCH_2, MATCH_3};
    std::vector<std::string> expected_maps = {MAP_1, MAP_2};
    EXPECT_EQ(information.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(information.matches, expected_matches)
            << "Received matches do not match expected matches.";
    EXPECT_EQ(information.maps, expected_maps) << "Received maps do not match expected maps.";

    // Envío de JOIN: partida deseada
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::JOIN, MATCH_3, PlayerType::COUNTERTERRORIST}))
            << "Sending match and map should succeed.";

    // Recepción de CONFIGURATION: tipo de terreno y id
    ActionDTO configuration = protocol.receive_and_deserialize_action();
    expected_type = ActionType::CONFIGURATION;
    TerrainType expected_terrain_type = MAP_1_TERRAIN_TYPE;
    uint16_t expected_id = ID_1;
    EXPECT_EQ(configuration.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(configuration.terrain_type, expected_terrain_type)
            << "Received terrain type does not match expected terrain.";
    EXPECT_EQ(configuration.id, expected_id) << "Received ID does not match expected ID.";

    protocol.kill();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
