add_library(connections STATIC TCPClient.hpp TCPClient.cpp)
target_link_libraries(connections PRIVATE spdlog::spdlog)
