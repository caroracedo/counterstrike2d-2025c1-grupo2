#ifndef MAIN_CPP
#define MAIN_CPP

#include <exception>
#include <iostream>

#include "client.h"

#define EXPECTED_ARGC 4

int main(int argc, char* argv[]) {
    try {
        // Momentáneamente agrego esto
        const char* hostname = NULL;
        const char* servname = NULL;
        const char* player_type = NULL;

        if (argc != EXPECTED_ARGC) {
            std::cerr << "Bad program call. Expected: " << argv[0] << " <hostname> <servname>\n";
            return EXIT_FAILURE;
        }
        if (std::string(argv[3]) != "Terrorist" && std::string(argv[3]) != "CounterTerrorist") {
            std::cerr << "Bad player type. Expected: Terrorist or CounterTerrorist\n";
            return EXIT_FAILURE;
        }
        hostname = argv[1];
        servname = argv[2];
        player_type = argv[3];

        Client client(hostname, servname, player_type);
        client.run();
        return EXIT_SUCCESS;

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

#endif
