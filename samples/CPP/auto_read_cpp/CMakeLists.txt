include_directories(${CMAKE_SOURCE_DIR}/Bindings/CPP/)

add_executable(auto_read_cpp main.cpp ${CMAKE_SOURCE_DIR}/Bindings/CPP/LGM5770Ucpp.cpp)
target_link_libraries(auto_read_cpp LGM5770U)