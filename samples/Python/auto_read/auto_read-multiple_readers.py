#!/usr/bin/env python3
import os
import sys
import signal
import threading
import datetime
import time

sys.path.append(os.path.realpath(os.path.join(os.path.dirname(__file__), "../../../Bindings/Python")))

import lgm5770u

cancelation_requested = False
safe_connect = True

if len(sys.argv) != 3:
    print("Usage: {} <device address> <device address>".format(sys.argv[0]), flush=True)
    exit(1)

address1 = sys.argv[1]
address2 = sys.argv[2]

threads = []
readers = []
cancelation_requested = False

def signal_handler(sig, frame):
    print("Signal '{}'".format(sig), flush=True)
    print("Exit requested.", flush=True)
    global cancelation_requested
    cancelation_requested = True
    for reader, address in readers:
        lgm5770u.cancel_read(reader)

def reader_loop(reader, address):
    print("Trying to connect to '{}'".format(address), flush=True)
    while 1:
        if cancelation_requested:
            break
        if not lgm5770u.is_connected(reader):
            ok, error = lgm5770u.connect(reader, address, safe_connect)
            if ok:
                print("Connected. Firmware version: '{}'".format(lgm5770u.get_firmware_version(reader)), flush=True)
            else:
                print("Can't connect, error: '{}'".format(error), flush=True)
                print("Trying to reconnect in 5s", flush=True)
                time.sleep(5)
                continue
        ok, card, error = lgm5770u.read(reader)
        if ok:
            if card:
                print(card, flush=True)
        else:
            print("Disconnected, error: '{}'".format(error), flush=True)
    lgm5770u.delete(reader)


def add_reader(address):
    reader = lgm5770u.new()
    readers.append((reader, address))


def start_readers():
    add_reader(address1)
    add_reader(address2)

    for reader, address in readers:
        th = threading.Thread(target=reader_loop, args=(reader, address))
        threads.append(th)
        th.start()


signal.signal(signal.SIGINT, signal_handler)

start_readers()

for thread in threads:
    thread.join()
