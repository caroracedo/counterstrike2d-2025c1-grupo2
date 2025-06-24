#include <exception>
#include <iostream>

#include "client.h"

#define EXPECTED_ARGC 1

int main(int argc, const char* argv[]) {
    try {
        if (argc != EXPECTED_ARGC) {
            std::cerr << "Bad program call. Expected: " << argv[0] << "\n";
            return EXIT_FAILURE;
        }

        Client client;
        client.run();
        return EXIT_SUCCESS;

    } catch (std::exception& e) {
        // If case of error, print it and exit with error
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
