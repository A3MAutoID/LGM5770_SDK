#include <iostream>
#include <sstream>
#include <thread>
#include "LGM5770U.h"

void logger(LogLevel level, const char* name, const char* msg) {
    std::stringstream ss;
    switch (level) {
        case LogLevel::Info: ss << "Info   "; break;
        case LogLevel::Warning: ss << "Warning"; break;
        case LogLevel::Error: ss << "Error  "; break;
    }
    ss << '\t' << name << '\t' << msg;
    std::cout << ss.str() << std::endl;
};

void OnConnected(LGM5770U* reader, const char* name, bool connected) {
    if (connected) {
        LGM5770U_set_auto_read_mode(reader);
        LGM5770U_set_auto_read_delay(reader, 500);
        logger(LogLevel::Info, name, "connected");
    } else
        logger(LogLevel::Error, name, "not connected");
};

int main(int argc, char* argv[]) {

    // raspberry
    auto address = "/dev/serial/by-path/platform-3f980000.usb-usb-0:1.3:1.0-port0";

    // linux
    //auto address = "/dev/serial/by-path/pci-0000:00:1d.1-usb-0:2:1.0-port0";

    auto reader = LGM5770U_new(address, "reader 1", OnConnected, logger);
    LGM5770U_connect(reader);

    char buffer[1024] = "";
    while (1) {
        LGM5770U_read(reader, buffer);

        if (buffer[0] != 0)
            std::cout << buffer << std::endl;
    }
}
