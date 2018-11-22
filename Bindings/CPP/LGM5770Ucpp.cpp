#include "LGM5770Ucpp.h"

LGM5770U::LGM5770U() {
    lgm5770U = detail::LGM5770U_new();
}

LGM5770U::~LGM5770U() {
    detail::LGM5770U_delete(lgm5770U);
}

bool LGM5770U::connect(std::string const& address, bool safe_connect, std::string& error) {
    char error_str[1024] = "";
    bool ok              = detail::LGM5770U_connect(lgm5770U, address.c_str(), safe_connect, error_str);
    error                = error_str;
    return ok;
}

bool LGM5770U::disconnect() {
    return detail::LGM5770U_disconnect(lgm5770U);
}

bool LGM5770U::is_connected() const {
    return detail::LGM5770U_is_connected(lgm5770U);
}

std::string LGM5770U::get_firmware_version() const {
    return detail::LGM5770U_get_firmware_version(lgm5770U);
}

bool LGM5770U::read(std::string& cardnumber, std::string& error) {
    char buffer[1024]    = "";
    char error_str[1024] = "";
    bool ok              = detail::LGM5770U_read(lgm5770U, buffer, error_str);
    cardnumber           = buffer;
    error                = error_str;
    return ok;
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

void LGM5770U::cancel_read() {
    detail::LGM5770U_cancel_read(lgm5770U);
}
