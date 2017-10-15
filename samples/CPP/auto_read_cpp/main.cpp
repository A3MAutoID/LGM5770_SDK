#include <iostream>
#include <sstream>
#include <thread>
#include "LGM5770Ucpp.h"

void logger(LGM5770U::LogLevel level, const char* name, const char* msg) {
    using LogLevel = LGM5770U::LogLevel;
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
    using LogLevel = LGM5770U::LogLevel;
    if (connected) {
        reader->set_auto_read_mode();
        reader->set_auto_read_delay(500);
        logger(LogLevel::Info, name, "connected");
    } else
        logger(LogLevel::Error, name, "not connected");
};

int main(int argc, char* argv[]) {

    // raspberry
    auto address = "/dev/serial/by-path/platform-3f980000.usb-usb-0:1.3:1.0-port0";

    // linux
    //auto address = "/dev/serial/by-path/pci-0000:00:1d.1-usb-0:2:1.0-port0";

    auto reader = LGM5770U{address, "reader 1", OnConnected, logger};
    reader.connect();

    while (1) {
        auto line = reader.read();

        if (!line.empty())
            std::cout << line << std::endl;
    }
}
