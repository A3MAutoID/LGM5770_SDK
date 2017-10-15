#pragma once
#include <map>
#include <string>

namespace detail {
#include <LGM5770U.h>
}

class LGM5770U {
public:
    enum class LogLevel {
        Info,
        Warning,
        Error,
    };

    using connected_cb_t = void (*)(LGM5770U* lgm5770U, const char* name, bool connected);
    using log_cb_t       = void (*)(LogLevel level, const char* name, const char* msg);

    LGM5770U(const char* address, const char* name, connected_cb_t connected_cb = nullptr, log_cb_t log_cb = nullptr);
    ~LGM5770U();

    const char* get_address() const;
    const char* get_name() const;
    bool        connect();
    bool        is_connected() const;
    void        set_auto_reconnect(bool is_reconnect);
    void        set_reconnect_delay(int delay_s);
    bool        restart();
    std::string read();
    void        set_read_one_mode();
    void        set_wait_mode();
    void        set_auto_read_mode();
    void        set_auto_read_delay(int delay_ms);
    void        wait_till_ready();
    void        cancel_read();

private:
    detail::LGM5770U* lgm5770U;
    connected_cb_t    connected_cb;

    static std::map<detail::LGM5770U*, LGM5770U*> lgms;
    static void                                   OnConnected(detail::LGM5770U* lgm5770U_c, const char* name, bool connected);
};
