// With multiple readers and cooperative read cancelation
// Using the CPP binding
#include <atomic>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>
#include "LGM5770Ucpp.h"

struct ReaderWrapper {
    std::string               address;
    std::unique_ptr<LGM5770U> reader;
    ReaderWrapper(std::string a, std::unique_ptr<LGM5770U>&& r)
        : address(std::move(a))
        , reader(std::move(r)) {}
};

int main(int argc, char* argv[]) {
    using namespace std::chrono;
    bool safe_connect = true;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <device address> <device address>" << std::endl;
        return 1;
    }
    std::cout << "Type 'quit' to exit." << std::endl;

    std::vector<ReaderWrapper> readers;
    readers.emplace_back(argv[1], std::make_unique<LGM5770U>());
    readers.emplace_back(argv[2], std::make_unique<LGM5770U>());

    auto              threads = std::vector<std::thread>{};
    std::atomic<bool> cancelation_requested{false};

    for (auto i_reader = 0; i_reader < readers.size(); i_reader++) {
        threads.emplace_back([i_reader, safe_connect, &readers, &cancelation_requested]() {
            auto const& address = readers[i_reader].address;
            auto&       reader  = *readers[i_reader].reader.get();
            std::string cardnumber;
            std::string error;
            std::cout << "Trying to connect to '" << address << "'" << std::endl;
            while (true) {
                if (cancelation_requested)
                    break;
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
        });
    }

    while (true) {
        std::string line;
        std::getline(std::cin, line);
        if (line == "quit") {
            std::cout << "Exit requested." << std::endl;
            cancelation_requested = true;
            for (auto& reader_wrapper : readers)
                reader_wrapper.reader->cancel_read();
            break;
        }
    }
    for (auto& th : threads)
        th.join();
}
