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
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
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
    EXPECT_TRUE(
            protocol.serialize_and_send_action({ActionType::CREATE, MATCH_3, MAP_1, NUM_TERRORISTS,
                                                NUM_COUNTERS, PlayerType::TERRORIST, SKIN}))
            << "Sending create should succeed.";

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
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
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
            {ActionType::JOIN, MATCH_3, PlayerType::COUNTERTERRORIST, SKIN}))
            << "Sending join should succeed.";

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

TEST(CLIENT_PROTOCOL, SHOP) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Recepción de SHOP: armas
    ActionDTO shop = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::SHOP;
    std::vector<WeaponModel> expected_weapons = {WEAPON_1, WEAPON_2};
    EXPECT_EQ(shop.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(shop.weapons, expected_weapons) << "Received weapons do not match expected weapons.";

    // Envío de WEAPON: arma deseada
    EXPECT_TRUE(protocol.serialize_and_send_action({ActionType::WEAPON, WEAPON_1}))
            << "Sending init should succeed.";

    // Envío de AMMOPRIMARY: cantidad de munición para arma primaria deseada
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::AMMOPRIMARY, static_cast<uint16_t>(AMMO)}))
            << "Sending ammo primary should succeed.";

    // Envío de AMMOSECONDARY: cantidad de munición para arma secundaria deseada
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::AMMOSECONDARY, static_cast<uint16_t>(AMMO)}))
            << "Sending ammo secondary should succeed.";

    protocol.kill();
}

TEST(CLIENT_PROTOCOL, UPDATE) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Recepción de UPDATE: objetos
    ActionDTO update = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::UPDATE;
    std::vector<ObjectDTO> expected_objects = {PLAYER_OBJECT,    OBSTACLE_OBJECT, BOMB_OBJECT,
                                               BOMB_ZONE_OBJECT, BULLET_OBJECT,   WEAPON_OBJECT};
    EXPECT_EQ(update.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(update.objects, expected_objects)
            << "Received objects do not match expected objects.";

    // Envío de MOVE: dirección deseada
    EXPECT_TRUE(protocol.serialize_and_send_action({ActionType::MOVE, Direction::UP}))
            << "Sending move should succeed.";

    // Envío de SHOOT: posición deseada
    EXPECT_TRUE(protocol.serialize_and_send_action({ActionType::SHOOT, POSITION}))
            << "Sending shoot should succeed.";

    // Envío de BOMB
    EXPECT_TRUE(protocol.serialize_and_send_action(ActionDTO{ActionType::BOMB}))
            << "Sending bomb should succeed.";

    // Envío de CHANGE
    EXPECT_TRUE(protocol.serialize_and_send_action(ActionDTO{ActionType::CHANGE}))
            << "Sending change should succeed.";

    // Envío de PICKUP
    EXPECT_TRUE(protocol.serialize_and_send_action(ActionDTO{ActionType::PICKUP}))
            << "Sending pickup should succeed.";

    // Envío de ROTATE: ángulo deseado
    EXPECT_TRUE(protocol.serialize_and_send_action(
            ActionDTO{ActionType::ROTATE, static_cast<float>(0)}))
            << "Sending rotate should succeed.";

    protocol.kill();
}

/* Test START QUIT y END */
TEST(CLIENT_PROTOCOL, START) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Envió de START
    EXPECT_TRUE(protocol.serialize_and_send_action(ActionDTO{ActionType::START}))
            << "Sending start should succeed.";

    protocol.kill();
}

TEST(CLIENT_PROTOCOL, QUIT) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Envió de QUIT
    EXPECT_TRUE(protocol.serialize_and_send_action(ActionDTO{ActionType::QUIT}))
            << "Sending quit should succeed.";

    protocol.kill();
}

TEST(CLIENT_PROTOCOL, MESSAGE) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Recepción de MESSAGE
    ActionDTO message = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::MESSAGE;
    WinnerTeamType expected_winner_team_type = WinnerTeamType::DRAW;
    EXPECT_EQ(message.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(message.winner_team_type, expected_winner_team_type)
            << "Received winner team type does not match expected winner team type.";

    protocol.kill();
}

TEST(CLIENT_PROTOCOL, END) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket socket(HOSTNAME, SERVNAME);
    ClientProtocol protocol(socket);

    // Recepción de END
    ActionDTO end = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::END;
    EXPECT_EQ(end.type, expected_type) << "Received type does not match expected type.";

    protocol.kill();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
