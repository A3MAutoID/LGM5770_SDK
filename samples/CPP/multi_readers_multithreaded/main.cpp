// With multiple readers and cooperative read cancelation
#include <atomic>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>
#include "LGM5770U.h"

// ensure memory is deallocated by using std::unique_ptr
using LGM5770U_ptr = std::unique_ptr<LGM5770U, void (*)(LGM5770U*)>;
inline LGM5770U_ptr make_LGM5770U() {
    return LGM5770U_ptr(LGM5770U_new(), LGM5770U_delete);
}

struct ReaderWrapper {
    std::string  address;
    LGM5770U_ptr reader;
    ReaderWrapper(std::string a, LGM5770U_ptr&& r)
        : address(std::move(a))
        , reader(std::move(r)) {}
};

int main(int argc, char* argv[]) {
    using namespace std::chrono;
    bool safe_connect = true;

    if (argc != 3) {
        char error[1024];
        sprintf(error, "Usage: %s <device address> <device address>", argv[0]);
        std::cerr << error << std::endl;
        return 1;
    }
    std::cout << "Type 'quit' to exit." << std::endl;

    std::vector<ReaderWrapper> readers;
    readers.emplace_back(argv[1], make_LGM5770U());
    readers.emplace_back(argv[2], make_LGM5770U());

    auto              threads = std::vector<std::thread>{};
    std::atomic<bool> cancelation_requested{false};

    for (auto i_reader = 0; i_reader < readers.size(); i_reader++) {
        threads.emplace_back([i_reader, safe_connect, &readers, &cancelation_requested]() {
            auto address = readers[i_reader].address.c_str();
            auto reader  = readers[i_reader].reader.get();

            char error[1024];
            char buffer[1024];

            std::cout << "Trying to connect to '" << address << "'" << std::endl;
            while (true) {
                if (cancelation_requested)
                    break;
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
        });
    }

    while (true) {
        std::string line;
        std::getline(std::cin, line);
        if (line == "quit") {
            std::cout << "Exit requested." << std::endl;
            cancelation_requested = true;
            for (auto& reader_wrapper : readers)
                LGM5770U_cancel_read(reader_wrapper.reader.get());
            break;
        }
    }
    for (auto& th : threads)
        th.join();
}
