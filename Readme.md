LGM5770 - Software Development Kit
====================================

This repository contains the library, header and samples to use the RFID readers of the LGM5770 serie.


Tested with:
- Ubuntu 16.04 Xenial with GCC 6.2
- Raspberry Raspbian Jessie Lite [2017-06-21](http://downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2017-06-23/2017-06-21-raspbian-jessie-lite.zip) with GCC 4.9.2


USB connection and reader address
---------------------------------

The LGM5770U readers are connected to USB ports. One or several readers can be connected at the same time.

Linux command to see the list of the connected readers:

     ls /dev/serial/by-path/

The device address will vary with host hardware but should be something like this (replace X with appropriate port number value):
- Linux PC:     `/dev/serial/by-path/pci-0000:00:1d.X-usb-0:2:1.0-port0`
- Raspberry Pi: `/dev/serial/by-path/platform-3f980000.usb-usb-0:1.X:1.0-port0`   

Under Linux and Raspberry, it is possible to connect to the reader with an address in the form `/dev/ttyUSBX`, but be aware that the port number `X` might change even if the device is connected to the same physical USB port. 


SDK layout
----------

This SDK is composed of several elements:

- The `LGM5770U` folder is the main component, it contains a header file and a static C library `libLGM5770U.a`.
- The `Bindings` folder contains C++ and Python bindings. The Python binding consists of a python source file `lgm5770u.py` and a shared library `_lgm5770u.so`. 
- The `LGM5770U_swig` folder contains the SWIG interface file to generate the Python binding. It can be used to generate bindings for other languages.
- The `samples` folder contains C++ and Python samples. The C++ samples are provided in 2 versions:
   - the `auto_read` and `multi_readers_multithreaded` samples use the raw C library  
   - the `auto_read_cpp` and `multi_readers_multithreaded_cpp` samples use the object oriented C++ binding  


Binary files
------------

To not clutter the Git repository, the binary files, `libLGM5770U.a` and `_lgm5770u.so`, are not included in the repository. They must be retrieved via the [release](https://github.com/A3MAutoID/LGM5770_SDK/releases) section. Choose the tar file appropriate to your system, Raspberry Pi and Linux version are provided.

Run the following commands to download and extract the archive (assuming v1.0.0 and Raspberry):
```
pi@raspberrypi ~ $ wget https://github.com/A3MAutoID/LGM5770_SDK/releases/download/v1.0.0/LGM5770_SDK.rpi.tar.gz
pi@raspberrypi ~ $ tar -xvf LGM5770_SDK.rpi.tar.gz
```


C++ samples
-------------

The address of the readers are hard-coded in the `main.cpp` of each sample with a Raspberry address by default. If under Linux PC, comment out the Raspberry address, uncomment the linux address sample and modify according to your system.      

Build the samples using standard CMake:
```
pi@raspberrypi ~/LGM5770_SDK $ mkdir build
pi@raspberrypi ~/LGM5770_SDK $ cd build
pi@raspberrypi ~/LGM5770_SDK/build $ cmake ..
pi@raspberrypi ~/LGM5770_SDK/build $ make
```

Then run any of the samples:
```
pi@raspberrypi ~/LGM5770_SDK/build $ ./samples/CPP/auto_read/auto_read
pi@raspberrypi ~/LGM5770_SDK/build $ ./samples/CPP/auto_read_cpp/auto_read_cpp
pi@raspberrypi ~/LGM5770_SDK/build $ ./samples/CPP/multi_readers_multithreaded/multi_readers
pi@raspberrypi ~/LGM5770_SDK/build $ ./samples/CPP/multi_readers_multithreaded_cpp/multi_readers_cpp
```


Python sample
-------------

```
pi@raspberrypi ~/LGM5770_SDK $ ./samples/Python/auto_read/auto_read.py
```


Build Python binding
--------------------

The Python binding can be regenerated using the provided SWIG interface, proceed as above with the `-DWITH_BINDINGS=ON`: 
```
pi@raspberrypi ~/LGM5770_SDK/build $ cmake .. -DWITH_BINDINGS=ON
pi@raspberrypi ~/LGM5770_SDK/build $ make
```
