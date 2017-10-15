find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/Bindings/CPP/)

add_executable(multi_readers_cpp main.cpp ${CMAKE_SOURCE_DIR}/Bindings/CPP/LGM5770Ucpp.cpp)
target_link_libraries(multi_readers_cpp ${CMAKE_THREAD_LIBS_INIT} LGM5770U)
