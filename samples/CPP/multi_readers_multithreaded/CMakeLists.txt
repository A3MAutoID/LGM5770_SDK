find_package(Threads REQUIRED)

add_executable(multi_readers main.cpp)
target_link_libraries(multi_readers ${CMAKE_THREAD_LIBS_INIT} LGM5770U)
