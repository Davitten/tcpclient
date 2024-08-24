#include "connections/TCPClient.hpp"
#include "protos/addressbook.pb.h"

int main(int argc, char *argv[]) {    

    con::TCPClient client("127.0.0.1", 29920);

    tutorial::Person a;
    a.set_id(2);
    a.set_name("Martin");
    a.set_email("ma.davidsen1@gmail.com");
    client.send_message("Test");
    client.send_message(a.SerializeAsString());
    return 0;
}