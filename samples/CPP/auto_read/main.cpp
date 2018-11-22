// Here no binding is used
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include "LGM5770U.h"


int main(int argc, char* argv[]) {
    using namespace std::chrono;
    bool safe_connect = true;
    char error[1024];
    char buffer[1024];

    if (argc != 2) {
        sprintf(error, "Usage: %s <device address>", argv[0]);
        std::cerr << error << std::endl;
        return 1;
    }

    auto address = argv[1];
    auto reader  = LGM5770U_new();

    std::cout << "Trying to connect to '" << address << "'" << std::endl;
    while (true) {
        if (!LGM5770U_is_connected(reader)) {
            if (LGM5770U_connect(reader, address, safe_connect, error))
                std::cout << "Connected. Firmware version: '" << LGM5770U_get_firmware_version(reader) << "'" << std::endl;
            else {
                std::cerr << "Can't connect, error: '" << error << "'" << std::endl;
                std::cout << "Trying to reconnect in 5s" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds{5});
                continue;
            }
        }
        if (LGM5770U_read(reader, buffer, error)) {
            if (buffer[0] != 0)
                std::cout << buffer << std::endl;
        } else
            std::cerr << "Disconnected, error: '" << error << "'" << std::endl;
    }
}
