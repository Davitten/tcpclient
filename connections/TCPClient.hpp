#pragma once
#include <string>

namespace con {
class TCPClient {
private:
    const std::string HOSTNAME_;
    const int PORT_;
    int clientSocketFD_;
public:
    TCPClient(const std::string& host, const int port);
    ~TCPClient();
    void send_message(const std::string& msg);
};
} // namespace con
