#pragma once
typedef void LGM5770U;

#ifdef __cplusplus
extern "C" {
#endif

enum LogLevel {
    Info,
    Warning,
    Error,
};
typedef void (*connected_cb_t)(LGM5770U* reader, const char* name, bool connected);
typedef void (*log_cb_t)(LogLevel level, const char* name, const char* msg);

LGM5770U*   LGM5770U_new(const char* address, const char* name, connected_cb_t connected_cb = nullptr, log_cb_t log_cb = nullptr);
const char* LGM5770U_get_address(LGM5770U* t);
const char* LGM5770U_get_name(LGM5770U* t);
void        LGM5770U_delete(LGM5770U* t);
bool        LGM5770U_connect(LGM5770U* t);
bool        LGM5770U_is_connected(const LGM5770U* t);
void        LGM5770U_set_auto_reconnect(LGM5770U* t, bool is_reconnect);
void        LGM5770U_set_reconnect_delay(LGM5770U* t, int delay_s);
bool        LGM5770U_restart(LGM5770U* t);
void        LGM5770U_read(LGM5770U* t, char* line);
void        LGM5770U_set_read_one_mode(LGM5770U* t);
void        LGM5770U_set_wait_mode(LGM5770U* t);
void        LGM5770U_set_auto_read_mode(LGM5770U* t);
void        LGM5770U_set_auto_read_delay(LGM5770U* t, int delay_ms);
void        LGM5770U_wait_till_ready(LGM5770U* t);
void        LGM5770U_cancel_read(LGM5770U* t);

#ifdef __cplusplus
}
#endif
