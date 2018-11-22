#pragma once
#include <string>

namespace detail {
#include <LGM5770U.h>
}

class LGM5770U {
    detail::LGM5770U* lgm5770U = nullptr;

public:
    LGM5770U();
    ~LGM5770U();

    LGM5770U(LGM5770U const&) = delete;
    LGM5770U& operator=(LGM5770U const&) = delete;

    LGM5770U(LGM5770U&&) = default;
    LGM5770U& operator=(LGM5770U&&) = default;

    bool        connect(std::string const& address, bool safe_connect, std::string& error);
    bool        disconnect();
    bool        is_connected() const;
    std::string get_firmware_version() const;
    bool        read(std::string& cardnumber, std::string& error);
    void        set_read_one_mode();
    void        set_wait_mode();
    void        set_auto_read_mode();
    void        set_auto_read_delay(int delay_ms);
    void        cancel_read();
};
