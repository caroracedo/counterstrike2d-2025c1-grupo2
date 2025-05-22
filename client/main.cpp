#ifndef MAIN_CPP
#define MAIN_CPP

#include <exception>
#include <iostream>

#include "client.h"

#define EXPECTED_ARGC 3

int main(int argc, char* argv[]) {
    try {
        // Momentáneamente agrego esto
        const char* hostname = NULL;
        const char* servname = NULL;

        if (argc != EXPECTED_ARGC) {
            std::cerr << "Bad program call. Expected: " << argv[0] << " <hostname> <servname>\n";
            return EXIT_FAILURE;
        }

        hostname = argv[1];
        servname = argv[2];

        Client client(hostname, servname);
        client.initiate_communication();
        return EXIT_SUCCESS;

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

#endif
