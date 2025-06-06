#ifndef MAIN_CPP
#define MAIN_CPP

#include <exception>
#include <iostream>

#include "client.h"

#define EXPECTED_ARGC 6

int main(int argc, char* argv[]) {
    try {
        // Momentáneamente agrego esto
        const char* hostname = NULL;
        const char* servname = NULL;
        const char* operation = NULL;
        const char* match = NULL;
        const char* player_type = NULL;

        if (argc != EXPECTED_ARGC) {
            std::cerr << "Bad program call. Expected: " << argv[0]
                      << " <hostname> <servname> <operation> <match> <player_type>\n";
            return EXIT_FAILURE;
        }
        if (std::string(argv[5]) != "Terrorist" && std::string(argv[5]) != "CounterTerrorist") {
            std::cerr << "Bad player type. Expected: Terrorist or CounterTerrorist\n";
            return EXIT_FAILURE;
        }
        if (std::string(argv[3]) != "Create" && std::string(argv[3]) != "Join") {
            std::cerr << "Bad operation. Expected: Create or Join\n";
            return EXIT_FAILURE;
        }
        hostname = argv[1];
        servname = argv[2];
        operation = argv[3];
        match = argv[4];
        player_type = argv[5];

        Client client(hostname, servname, operation, match, player_type);
        client.run();
        return EXIT_SUCCESS;

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

#endif
