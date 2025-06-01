#include <cstring>
#include <exception>
#include <iostream>

#include "server.h"

#define EXPECTED_ARGC 2

int main(int argc, char* argv[]) {
    try {
        const char* port = nullptr;

        if (argc != EXPECTED_ARGC) {
            std::cerr << "Bad program call. Expected: " << argv[0] << " <port> <protocol-type>\n";
            return EXIT_FAILURE;
        }

        port = argv[1];

        Server server(port);
        server.run();
        return EXIT_SUCCESS;

    } catch (const std::exception& err) {
        std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        return EXIT_FAILURE;
    }
}
