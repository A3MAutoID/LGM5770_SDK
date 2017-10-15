set (CMAKE_CXX_STANDARD 14)

link_directories(${CMAKE_SOURCE_DIR}/LGM5770U/)
include_directories(${CMAKE_SOURCE_DIR}/LGM5770U/)

add_subdirectory(auto_read)
add_subdirectory(auto_read_cpp)
add_subdirectory(multi_readers_multithreaded)
add_subdirectory(multi_readers_multithreaded_cpp)
