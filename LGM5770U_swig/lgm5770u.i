%module("threads"=1) lgm5770u

%include "cstring.i"

%{
    #include "LGM5770U.h"
%}


// https://gist.github.com/sterin/32b5c7c42a177dacf455
// a typemap for the callback, it expects the argument to be an integer
// whose value is the address of an appropriate callback function
%typemap(in) connected_cb_t {
    $1 = (connected_cb_t)PyInt_AsLong($input);
}
%typemap(in) log_cb_t {
    $1 = (log_cb_t)PyInt_AsLong($input);
}

%cstring_bounded_output(char *line, 1024);

%rename(get_address)         LGM5770U_get_address;
%rename(get_name)            LGM5770U_get_name;
%rename(delete)              LGM5770U_delete;
%rename(connect)             LGM5770U_connect;            
%rename(is_connected)        LGM5770U_is_connected;       
%rename(set_auto_reconnect)  LGM5770U_set_auto_reconnect; 
%rename(set_reconnect_delay) LGM5770U_set_reconnect_delay;
%rename(restart)             LGM5770U_restart;            
%rename(read)                LGM5770U_read;               
%rename(set_read_one_mode)   LGM5770U_set_read_one_mode;  
%rename(set_wait_mode)       LGM5770U_set_wait_mode;      
%rename(set_auto_read_mode)  LGM5770U_set_auto_read_mode; 
%rename(set_auto_read_delay) LGM5770U_set_auto_read_delay;
%rename(wait_till_ready)     LGM5770U_wait_till_ready;    
%rename(cancel_read)         LGM5770U_cancel_read;        


%include "LGM5770U.h"
%pythoncode
%{

import ctypes

py_connected_cb_type = ctypes.CFUNCTYPE(None, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_bool)
py_log_cb_type = ctypes.CFUNCTYPE(None, ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p)

def new(address, name, on_connected = None, on_log = None):
    if on_connected:
        f = py_connected_cb_type(on_connected)
        f_ptr = ctypes.cast(f, ctypes.c_void_p).value
    else:
        f_ptr = 0
    if on_log:
        g = py_log_cb_type(on_log)
        g_ptr = ctypes.cast(g, ctypes.c_void_p).value
    else:
        g_ptr = 0
    return _lgm5770u.LGM5770U_new(address, name, f_ptr, g_ptr)
%}
