#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <iostream>
#include <string>
#include <arpa/inet.h> // inet_aton
#include <unistd.h> // close socket

#include "TCPClient.hpp"

namespace con {
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
        std::cout << "Error connecting to socket!"<< std::endl;
        throw std::runtime_error("Error connecting to socket!");
    }
    std::cout << "Connection to host: " << HOSTNAME_ << " on port: " << PORT_ << " established!" << std::endl;
}

TCPClient::~TCPClient()
{
    close(clientSocketFD_);
    std::cout << "Connection closed!" << std::endl;
}

void TCPClient::send_message(const std::string& msg) {
    // Convert from host to network long
    uint32_t size_of_msg = htonl(std::size(msg));
    send(clientSocketFD_, &size_of_msg, sizeof(uint32_t), 0);
    send(clientSocketFD_, msg.c_str(), std::size(msg), 0);
}
} // namespace con