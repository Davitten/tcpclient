#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <iostream>
#include <string>
#include <arpa/inet.h> // inet_aton
#include <unistd.h> // close socket

#include "TCPClient.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <string>

using namespace std::string_literals;

namespace con {
const std::string LOGGER_NAME = "TCPClient"s;
auto logger = spdlog::stdout_color_mt(LOGGER_NAME);
TCPClient::TCPClient(const std::string& host, const int port) : HOSTNAME_(host), PORT_(port)
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    // Set port number
    serverAddress.sin_port = htons(PORT_);
    // set ip address from dottet string
    int valid_ip = inet_aton(HOSTNAME_.c_str(), &serverAddress.sin_addr);
    if(valid_ip == 0) {
        std::cout << "Invalid hostname: " << HOSTNAME_ << std::endl;
        throw std::runtime_error("Invalid hostname!");
    }
    // serverAddress.sin_addr.s_addr = INADDR_ANY;  THIS IS ONLY WHEN USING ALL IP ADDRESSES

    clientSocketFD_ = socket(AF_INET, SOCK_STREAM, 0);

    int status = connect(clientSocketFD_, (sockaddr*) &serverAddress, sizeof(serverAddress));
    if(status < 0)
    {
        std::cerr << "Error connecting to socket!"<< std::endl;
        throw std::runtime_error("Error connecting to socket!");
    }
    spdlog::get(LOGGER_NAME)->info("Connection to host: {} on port: {} established!", HOSTNAME_, PORT_);
}

TCPClient::~TCPClient()
{
    close(clientSocketFD_);
    spdlog::get(LOGGER_NAME)->info("Connection closed!");
}

void TCPClient::send_message(const std::string& msg) {
    send(clientSocketFD_, msg.c_str(), std::size(msg), 0);
}

// void TCPClient::send_message(const std::string& msg) {
//     // Convert from host to network long
//     uint32_t size_of_msg = htonl(std::size(msg));
//     send(clientSocketFD_, &size_of_msg, sizeof(uint32_t), 0);
//     send(clientSocketFD_, msg.c_str(), std::size(msg), 0);
// }
} // namespace con