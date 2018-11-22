// Using the CPP binding
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include "LGM5770Ucpp.h"

int main(int argc, char* argv[]) {
    using namespace std::chrono;
    bool safe_connect = true;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <device address>" << std::endl;
        return 1;
    }

    auto address = argv[1];
    auto reader  = LGM5770U{};

    std::string cardnumber;
    std::string error;
    std::cout << "Trying to connect to '" << address << "'" << std::endl;
    while (true) {
        if (!reader.is_connected()) {
            if (reader.connect(address, safe_connect, error))
                std::cout << "Connected. Firmware version: '" << reader.get_firmware_version() << "'" << std::endl;
            else {
                std::cerr << "Can't connect, error: '" << error << "'" << std::endl;
                std::cout << "Trying to reconnect in 5s" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds{5});
                continue;
            }
        }
        if (reader.read(cardnumber, error)) {
            if (!cardnumber.empty())
                std::cout << cardnumber << std::endl;
        } else
            std::cerr << "Disconnected, error: '" << error << "'" << std::endl;
    }
}
