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
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Envío de INFORMATION: partidas y mapas
    std::vector<std::string> matches = {MATCH_1, MATCH_2};
    std::vector<std::string> maps = {MAP_1, MAP_2};
    EXPECT_TRUE(protocol.serialize_and_send_action({ActionType::INFORMATION, matches, maps}))
            << "Sending information should succeed.";

    // Recepción de CREATE: partida y mapa deseados
    ActionDTO create = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::CREATE;
    std::string expected_match = MATCH_3;
    std::string expected_map = MAP_1;
    PlayerType expected_player_type = PlayerType::TERRORIST;
    PlayerSkin expected_player_skin = SKIN;
    u_int16_t expected_num_terrorists = NUM_TERRORISTS;
    uint16_t expected_num_counters = NUM_COUNTERS;
    EXPECT_EQ(create.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(create.match, expected_match) << "Received match do not match expected match.";
    EXPECT_EQ(create.map, expected_map) << "Received map do not match expected map.";
    EXPECT_EQ(create.player_type, expected_player_type)
            << "Received player type do not match expected player type.";
    EXPECT_EQ(create.player_skin, expected_player_skin)
            << "Received player skin does not match expected player skin.";
    EXPECT_EQ(create.number_terrorist, expected_num_terrorists)
            << "Received number terrorists do not match expected number terrorists.";
    EXPECT_EQ(create.number_counterterrorist, expected_num_counters)
            << "Received number counters do not match expected number counters.";

    // Envío de CONFIGURATION: tipo de terreno y id
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::CONFIGURATION, MAP_1_TERRAIN_TYPE, ID_1}))
            << "Sending configuration should succeed.";

    protocol.kill();
}

TEST(SERVER_PROTOCOL, LOBBY_JOIN) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
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
    PlayerSkin expected_player_skin = SKIN;
    EXPECT_EQ(join.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(join.match, expected_match) << "Received match do not match expected match.";
    EXPECT_EQ(join.player_type, expected_player_type)
            << "Received player type do not match expected player type.";
    EXPECT_EQ(join.player_skin, expected_player_skin)
            << "Received player skin do not match expected player skin.";

    // Envío de CONFIGURATION: tipo de terreno y id
    EXPECT_TRUE(protocol.serialize_and_send_action(
            {ActionType::CONFIGURATION, MAP_1_TERRAIN_TYPE, ID_1}))
            << "Sending configuration should succeed.";

    protocol.kill();
}

TEST(SERVER_PROTOCOL, SHOP) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Envío de SHOP: armas
    EXPECT_TRUE(protocol.serialize_and_send_action({ActionType::SHOP, {WEAPON_1, WEAPON_2}}))
            << "Sending shop should succeed.";

    // Recepción de WEAPON: arma deseada
    ActionDTO weapon = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::WEAPON;
    WeaponModel expected_weapon = WEAPON_1;
    uint16_t expected_id1 = ID_1;
    EXPECT_EQ(weapon.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(weapon.weapon, expected_weapon) << "Received weapon do not match expected weapon.";
    EXPECT_EQ(weapon.id, expected_id1) << "Received id does not match expected id";

    // Recepción de AMMOPRIMARY: cantidad de munición para arma primaria deseada
    ActionDTO ammo_primary = protocol.receive_and_deserialize_action();
    expected_type = ActionType::AMMOPRIMARY;
    uint16_t expected_ammo = AMMO;
    uint16_t expected_id2 = ID_1;
    EXPECT_EQ(ammo_primary.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(ammo_primary.ammo, expected_ammo) << "Received ammo do not match expected ammo.";
    EXPECT_EQ(ammo_primary.id, expected_id2) << "Received id does not match expected id";

    // Recepción de AMMOSECONDARY: cantidad de munición para arma secundaria deseada
    ActionDTO ammo_secondary = protocol.receive_and_deserialize_action();
    expected_type = ActionType::AMMOSECONDARY;
    expected_ammo = AMMO;
    uint16_t expected_id3 = ID_1;
    EXPECT_EQ(ammo_secondary.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(ammo_secondary.ammo, expected_ammo) << "Received ammo do not match expected ammo.";
    EXPECT_EQ(ammo_secondary.id, expected_id3) << "Received id does not match expected id";

    protocol.kill();
}

TEST(SERVER_PROTOCOL, UPDATE) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Envío de UPDATE: objetos
    std::vector<ObjectDTO> objects = {PLAYER_OBJECT,    OBSTACLE_OBJECT, BOMB_OBJECT,
                                      BOMB_ZONE_OBJECT, BULLET_OBJECT,   WEAPON_OBJECT};
    EXPECT_TRUE(protocol.serialize_and_send_action({ActionType::UPDATE, objects}))
            << "Sending update should succeed.";

    // Recepción de MOVE: dirección deseada
    ActionDTO move = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::MOVE;
    Direction expected_direction = Direction::UP;
    uint16_t expected_id1 = ID_1;
    EXPECT_EQ(move.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(move.direction, expected_direction)
            << "Received direction do not match expected direction.";
    EXPECT_EQ(move.id, expected_id1) << "Received id does not match expected id";

    // Recepción de SHOOT: posición deseada
    ActionDTO shoot = protocol.receive_and_deserialize_action();
    expected_type = ActionType::SHOOT;
    std::vector<uint16_t> expected_desired_position = POSITION;
    uint16_t expected_id2 = ID_1;
    EXPECT_EQ(shoot.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(shoot.desired_position, expected_desired_position)
            << "Received desired_position do not match expected desired_position.";
    EXPECT_EQ(shoot.id, expected_id2) << "Received id does not match expected id";

    // Recepción de BOMB
    ActionDTO bomb = protocol.receive_and_deserialize_action();
    expected_type = ActionType::BOMB;
    uint16_t expected_id3 = ID_1;
    EXPECT_EQ(bomb.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(bomb.id, expected_id3) << "Received id does not match expected id";

    // Recepción de CHANGE
    ActionDTO change = protocol.receive_and_deserialize_action();
    expected_type = ActionType::CHANGE;
    uint16_t expected_id4 = ID_1;
    EXPECT_EQ(change.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(change.id, expected_id4) << "Received id does not match expected id";

    // Recepción de BOMB
    ActionDTO pickup = protocol.receive_and_deserialize_action();
    expected_type = ActionType::PICKUP;
    uint16_t expected_id5 = ID_1;
    EXPECT_EQ(pickup.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(pickup.id, expected_id5) << "Received id does not match expected id";

    // Recepción de ROTATE
    ActionDTO rotate = protocol.receive_and_deserialize_action();
    expected_type = ActionType::ROTATE;
    float expected_angle = 0;
    uint16_t expected_id6 = ID_1;
    EXPECT_EQ(rotate.type, expected_type) << "Received type does not match expected type.";
    EXPECT_EQ(rotate.angle, expected_angle) << "Received angle does not match expected angle.";
    EXPECT_EQ(rotate.id, expected_id6) << "Received id does not match expected id";

    protocol.kill();
}

/* Test START QUIT y END */
TEST(SERVER_PROTOCOL, START) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Recepción de START
    ActionDTO start = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::START;
    uint16_t expected_id = ID_1;
    EXPECT_EQ(start.type, expected_type) << "Received type does not match expected type";
    EXPECT_EQ(start.id, expected_id) << "Received id does not match expected id";

    protocol.kill();
}

TEST(SERVER_PROTOCOL, QUIT) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Recepción de QUIT
    ActionDTO quit = protocol.receive_and_deserialize_action();
    ActionType expected_type = ActionType::QUIT;
    uint16_t expected_id = ID_1;
    EXPECT_EQ(quit.type, expected_type) << "Received type does not match expected type";
    EXPECT_EQ(quit.id, expected_id) << "Received id does not match expected id";

    protocol.kill();
}

TEST(SERVER_PROTOCOL, MESSAGE) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Envío de MESSAGE
    EXPECT_TRUE(protocol.serialize_and_send_action({ActionType::MESSAGE, WinnerTeamType::DRAW}))
            << "Sending message should succeed.";

    protocol.kill();
}

TEST(SERVER_PROTOCOL, END) {
    std::this_thread::sleep_for(std::chrono::milliseconds(TIME));
    Socket server_socket(SERVNAME);
    Socket client_socket(server_socket.accept());
    ServerProtocol protocol(client_socket, ID_1);

    // Envío de END
    EXPECT_TRUE(protocol.serialize_and_send_action(ActionDTO{ActionType::END}))
            << "Sending end should succeed.";

    protocol.kill();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
