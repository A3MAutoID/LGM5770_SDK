#include "LGM5770Ucpp.h"

std::map<detail::LGM5770U*, LGM5770U*> LGM5770U::lgms;

void LGM5770U::OnConnected(detail::LGM5770U* lgm5770U_c, const char* name, bool connected) {
    auto lgm5770U = lgms[lgm5770U_c];
    if (lgm5770U->connected_cb)
        lgm5770U->connected_cb(lgm5770U, name, connected);
}

LGM5770U::LGM5770U(const char* address, const char* name, connected_cb_t connected_cb, log_cb_t log_cb) {
    this->connected_cb = connected_cb;
    using log_cb_c_t   = void (*)(detail::LogLevel, const char* name, const char* msg);
    auto log_cb_c      = reinterpret_cast<log_cb_c_t>(log_cb);
    lgm5770U           = detail::LGM5770U_new(address, name, OnConnected, log_cb_c);
    lgms[lgm5770U]     = this;
}

LGM5770U::~LGM5770U() {
    detail::LGM5770U_delete(lgm5770U);
    lgms.erase(this);
}

const char* LGM5770U::get_address() const {
    return detail::LGM5770U_get_address(lgm5770U);
}

const char* LGM5770U::get_name() const {
    return detail::LGM5770U_get_name(lgm5770U);
}

bool LGM5770U::connect() {
    return detail::LGM5770U_connect(lgm5770U);
}

bool LGM5770U::is_connected() const {
    return detail::LGM5770U_is_connected(lgm5770U);
}

void LGM5770U::set_auto_reconnect(bool is_reconnect) {
    detail::LGM5770U_set_auto_reconnect(lgm5770U, is_reconnect);
}

void LGM5770U::set_reconnect_delay(int delay_s) {
    detail::LGM5770U_set_reconnect_delay(lgm5770U, delay_s);
}

bool LGM5770U::restart() {
    return detail::LGM5770U_restart(lgm5770U);
}

std::string LGM5770U::read() {
    char buffer[1024] = "";
    detail::LGM5770U_read(lgm5770U, buffer);
    return buffer;
}

void LGM5770U::set_read_one_mode() {
    detail::LGM5770U_set_read_one_mode(lgm5770U);
}

void LGM5770U::set_wait_mode() {
    detail::LGM5770U_set_wait_mode(lgm5770U);
}

void LGM5770U::set_auto_read_mode() {
    detail::LGM5770U_set_auto_read_mode(lgm5770U);
}

void LGM5770U::set_auto_read_delay(int delay_ms) {
    detail::LGM5770U_set_auto_read_delay(lgm5770U, delay_ms);
}

void LGM5770U::wait_till_ready() {
    detail::LGM5770U_wait_till_ready(lgm5770U);
}

void LGM5770U::cancel_read() {
    detail::LGM5770U_cancel_read(lgm5770U);
}
