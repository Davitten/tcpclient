cmake_minimum_required(VERSION 3.10)

project(tcpclient)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_subdirectory(connections)

add_executable(main main.cpp protos/addressbook.pb.cc)

target_link_libraries(main PUBLIC connections)
target_link_libraries(main PRIVATE ${PROTOBUF_LIBRARY})

target_include_directories(main PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}"
                          )