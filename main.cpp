#include <iostream>
#include <cstdlib>
#include "server.hpp"

int main(int argc, char *argv[]) {
    try {
        if (argc != 4) {
            std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 .\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 .\n";
            return EXIT_FAILURE;
        }

        sashi_tiny_http::Server server(argv[1], argv[2], argv[3]);

        server.Run();
    }
    catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}