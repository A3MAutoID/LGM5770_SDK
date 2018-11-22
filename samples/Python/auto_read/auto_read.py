#!/usr/bin/env python3
import os
import sys
import signal
import time

sys.path.append(os.path.realpath(os.path.join(os.path.dirname(__file__), "../../../Bindings/Python")))

import lgm5770u

cancelation_requested = False
safe_connect = True

if len(sys.argv) != 2:
    print("Usage: {} <device address>".format(sys.argv[0]), flush=True)
    exit(1)


address = sys.argv[1]
reader  = lgm5770u.new()

def signal_handler(sig, frame):
    print("Signal '{}'".format(sig), flush=True)
    print("Exit requested.", flush=True)
    global cancelation_requested
    cancelation_requested = True
    lgm5770u.cancel_read(reader)

signal.signal(signal.SIGINT, signal_handler)

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
