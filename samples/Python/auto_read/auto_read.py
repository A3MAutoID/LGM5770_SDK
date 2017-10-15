#!/usr/bin/env python3
import os
import sys
import signal
import threading
import datetime
from enum import Enum

sys.path.append(os.path.realpath(os.path.join(os.path.dirname(__file__), "../../../Bindings/Python")))

import lgm5770u

threads = []
readers = []
reader_names = {}  # workaround to retrieve reader from its name in 'on_connected' call_back
cancelation_requested = False
lock = threading.RLock()  # we must lock on re-entering python code when called from within c

# raspberry
address1 = '/dev/serial/by-path/platform-3f980000.usb-usb-0:1.3:1.0-port0'
address2 = '/dev/serial/by-path/platform-3f980000.usb-usb-0:1.5:1.0-port0'

# linux
# address1 = '/dev/serial/by-path/pci-0000:00:1d.1-usb-0:2:1.0-port0'
# address2 = '/dev/serial/by-path/pci-0000:00:1d.0-usb-0:2:1.0-port0'


class LogLevel(Enum):
    Info = lgm5770u.Info
    Warning = lgm5770u.Warning
    Error = lgm5770u.Error


def signal_handler(sig, frame):
    print("received signal '{}'".format(sig))
    global cancelation_requested
    cancelation_requested = True
    for reader in readers:
        lgm5770u.cancel_read(reader)


def decode(line):
    return line.decode().rstrip()


def on_connected(reader_not_usable_in_python, name, connected):
    with lock:
        reader = reader_names[name.decode()]
        if connected:
            lgm5770u.set_auto_read_mode(reader)
            on_log(lgm5770u.Info, name, "connected".encode())
        else:
            on_log(lgm5770u.Error, name, "not connected".encode())


def on_log(level, name, msg):
    with lock:
        print(
            "{}, {}, {}, {}, {}".format(datetime.datetime.now(), threading.current_thread().ident, LogLevel(level).name,
                                        decode(name), decode(msg)), flush=True)


def reader_loop(reader):
    if lgm5770u.connect(reader):
        while 1:
            card = lgm5770u.read(reader)
            if cancelation_requested:
                break
            if card:
                print(card)

    lgm5770u.delete(reader)


def add_reader(address, name, on_connected_cb=None, on_log_cb=None):
    reader = lgm5770u.new(address, name, on_connected_cb, on_log_cb)
    readers.append(reader)
    reader_names[name] = reader


def start_readers():
    add_reader(address1, "reader 1", on_connected, on_log)
    add_reader(address2, "reader 2", on_connected, on_log)

    for reader in readers:
        th = threading.Thread(target=reader_loop, args=(reader,), name=lgm5770u.get_name(reader))
        threads.append(th)
        th.start()


signal.signal(signal.SIGINT, signal_handler)

start_readers()

for thread in threads:
    thread.join()
