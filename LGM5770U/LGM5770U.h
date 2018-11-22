#pragma once
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LGM5770U LGM5770U;

LGM5770U*   LGM5770U_new();
void        LGM5770U_delete(LGM5770U* t);
bool        LGM5770U_connect(LGM5770U* t, const char* address, bool safe_connect, char* error);
bool        LGM5770U_disconnect(LGM5770U* t);
bool        LGM5770U_is_connected(LGM5770U* t);
const char* LGM5770U_get_firmware_version(LGM5770U* t);
bool        LGM5770U_read(LGM5770U* t, char* buf, char* error);
void        LGM5770U_set_read_one_mode(LGM5770U* t);
void        LGM5770U_set_wait_mode(LGM5770U* t);
void        LGM5770U_set_auto_read_mode(LGM5770U* t);
void        LGM5770U_set_auto_read_delay(LGM5770U* t, int delay_ms);
void        LGM5770U_cancel_read(LGM5770U* t);

#ifdef __cplusplus
}
#endif
