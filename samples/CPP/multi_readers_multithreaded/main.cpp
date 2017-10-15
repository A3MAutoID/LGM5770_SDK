#include <atomic>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>
#include "LGM5770U.h"

using LGM5770U_ptr = std::unique_ptr<LGM5770U, void (*)(LGM5770U*)>;
inline LGM5770U_ptr make_LGM5770U(const char* address, const char* name, connected_cb_t connected_cb, log_cb_t log_cb) {
    return LGM5770U_ptr(LGM5770U_new(address, name, connected_cb, log_cb), LGM5770U_delete);
}

std::map<LGM5770U*, std::function<void(LGM5770U*)>> setup_readers;
std::mutex                                          cout_mtx;

void logger(LogLevel level, const char* name, const char* msg) {
    std::stringstream ss;
    switch (level) {
        case LogLevel::Info: ss << "Info   "; break;
        case LogLevel::Warning: ss << "Warning"; break;
        case LogLevel::Error: ss << "Error  "; break;
    }
    ss << '\t' << name << '\t' << msg;
    std::lock_guard<std::mutex> lk{cout_mtx};
    std::cout << ss.str() << std::endl;
};

void OnConnected(LGM5770U* reader, const char* name, bool connected) {
    std::stringstream ss;
    if (connected) {
        setup_readers[reader](reader);
        logger(LogLevel::Info, name, "connected");
    } else
        logger(LogLevel::Error, name, "not connected");
};

int main(int argc, char* argv[]) {
    using namespace std::chrono;

    // raspberry
    auto address1 = "/dev/serial/by-path/platform-3f980000.usb-usb-0:1.3:1.0-port0";
    auto address2 = "/dev/serial/by-path/platform-3f980000.usb-usb-0:1.5:1.0-port0";

    // linux
    //auto address1 = "/dev/serial/by-path/pci-0000:00:1d.1-usb-0:2:1.0-port0";
    //auto address2 = "/dev/serial/by-path/pci-0000:00:1d.0-usb-0:2:1.0-port0";

    // for sample of set_auto_reconnect to false
    auto address3 = "/unvalid_address";

    std::vector<LGM5770U_ptr> readers;
    readers.emplace_back(make_LGM5770U(address1, "reader 1", OnConnected, logger));
    readers.emplace_back(make_LGM5770U(address2, "reader 2", OnConnected, logger));
    readers.emplace_back(make_LGM5770U(address3, "reader 3", OnConnected, logger));

    // change some readers parameters before connecting
    LGM5770U_set_reconnect_delay(readers[1].get(), 7);
    LGM5770U_set_auto_reconnect(readers[2].get(), false);

    // setup some other parameters change after connection is made
    setup_readers[readers[0].get()] = [](LGM5770U*) {};
    setup_readers[readers[1].get()] = [](LGM5770U* reader) { LGM5770U_set_reconnect_delay(reader, 10); };
    setup_readers[readers[2].get()] = [](LGM5770U*) {};

    auto              threads = std::vector<std::thread>{};
    std::atomic<bool> cancelation_requested{false};

    for (auto i_reader = 0; i_reader < readers.size(); i_reader++) {
        threads.emplace_back([i_reader, &readers, &cancelation_requested]() {
            LGM5770U* reader = readers[i_reader].get();

            if (LGM5770U_connect(reader)) {
                char buffer[1024] = "";
                while (1) {
                    LGM5770U_set_read_one_mode(reader);  // in read_one_mode, reader goes into wait mode after reading a card, so we need to reset to read_one mode at each iteration
                    LGM5770U_read(reader, buffer);
                    //                reader.set_wait_mode();  // force wait mode for cases where read has been canceled by exterior event

                    if (cancelation_requested)
                        break;

                    //                if (reader.is_restart) {
                    //                    reader.is_read_allowed = true;
                    //                    reader.wait_till_ready();
                    //                    reader.restart();
                    //                    reader.is_restart = false;
                    //                    continue;
                    //                }

                    if (buffer[0] != 0) {
                        std::cout << buffer << std::endl;

                        // here we can vary delay before reading another Card depending on some criteria
                        if (buffer[0] % 2 == 0)
                            std::this_thread::sleep_for(std::chrono::seconds{1});
                        else
                            std::this_thread::sleep_for(std::chrono::seconds{3});
                    }
                }
            }
        });
    }
    while (true) {
        std::string line;
        std::getline(std::cin, line);
        if (line == "quit") {
            cancelation_requested = true;
            for (auto& reader_ptr : readers)
                LGM5770U_cancel_read(reader_ptr.get());
            break;
        }
        //        if (line == "restart") {
        //            for (auto& hw_reader_ptr : readers) {
        //                hw_reader_ptr->is_restart      = true;
        //                hw_reader_ptr->is_read_allowed = false;
        //            }
        //        }
    }
    for (auto& th : threads)
        th.join();
}
