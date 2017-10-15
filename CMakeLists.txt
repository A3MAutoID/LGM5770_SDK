cmake_minimum_required(VERSION 3.2)
project(LGM5770U)

OPTION(WITH_BINDINGS "Create the bindings, otherwise use the prebuilt provided ones" OFF)


if (WITH_BINDINGS)
    add_subdirectory(LGM5770U_swig)
endif ()

add_subdirectory(samples/CPP)
