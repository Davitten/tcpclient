cmake_minimum_required(VERSION 3.10)

project(tcpclient)

set(CMAKE_CXX_STANDARD 20)

find_package(spdlog REQUIRED)

add_subdirectory(connections)
add_subdirectory(protos)

add_executable(main main.cpp)

target_link_libraries(main PUBLIC connections)
target_link_libraries(main PUBLIC protos)
target_link_libraries(main PRIVATE spdlog::spdlog)

target_include_directories(main PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}"
                          )