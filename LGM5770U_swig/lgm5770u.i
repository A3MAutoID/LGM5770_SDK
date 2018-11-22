%module("threads"=1) lgm5770u

%include "cstring.i"

%{
    #include "LGM5770U.h"
%}

%cstring_bounded_output(char *buf, 1024);
%cstring_bounded_output(char *error, 1024);

%rename(new)                  LGM5770U_new;
%rename(delete)               LGM5770U_delete;
%rename(connect)              LGM5770U_connect;
%rename(disconnect)           LGM5770U_disconnect;
%rename(is_connected)         LGM5770U_is_connected;
%rename(get_firmware_version) LGM5770U_get_firmware_version;
%rename(read)                 LGM5770U_read;
%rename(set_read_one_mode)    LGM5770U_set_read_one_mode;
%rename(set_wait_mode)        LGM5770U_set_wait_mode;
%rename(set_auto_read_mode)   LGM5770U_set_auto_read_mode;
%rename(set_auto_read_delay)  LGM5770U_set_auto_read_delay;
%rename(cancel_read)          LGM5770U_cancel_read;

%include "LGM5770U.h"
