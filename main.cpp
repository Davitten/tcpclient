#include "connections/TCPClient.hpp"
#include "protos/addressbook.pb.h"
#include <filesystem>
#include <fstream> 
#include <source_location>
#include "google/protobuf/util/delimited_message_util.h"

namespace fs = std::filesystem;
// Custom output of tutorial::Person. Preferably use .DebugString()
std::ostream& operator<<(std::ostream& os, const tutorial::Person& b)
{
    os << "name: " << b.name()
        << "\nid: " << b.id() 
        << "\nemail: " << b.email()
        << "\nphones:";
    for(auto& phone : b.phones()) {
            // add new line for all but first phone
            if(&phone != &(*b.phones().begin()))
                os << "\n";
            os << "\n\tnumber: " << phone.number()
            << "\n\ttype: " << phone.type();
    }    
    return os;
}

int main(int argc, char *argv[]) {    
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    con::TCPClient client("127.0.0.1", 29920);

    // Create data to send
    tutorial::Person a;
    a.set_name("Martin");
    a.set_id(2);
    a.set_email("ma.davidsen1@gmail.com");
    auto* phone_number = a.add_phones();
    phone_number->set_number("42 61 67 03");
    phone_number->set_type(tutorial::Person::PHONE_TYPE_MOBILE);

    phone_number = a.add_phones();
    phone_number->set_number("89 93 89 89");
    phone_number->set_type(tutorial::Person::PHONE_TYPE_WORK);

    std::cout << "Before Serialization\n" << a.DebugString() << std::endl;

    // Get path to the the current source location
    fs::path current_path = std::source_location::current().file_name();
    fs::path output_file_path = current_path.parent_path() / "test.txt";

    std::cout << "Writing to file: " << output_file_path << std::endl;

    std::ofstream file(output_file_path); 
    if(file.is_open()) {
        file << "As String:\n"
        << a.DebugString() << "\n"
        << "As Serialized:";
        a.SerializeToOstream(&file);
        file.close();
    }
    else {
        std::cerr << "Failed to write file" << std::endl;
    }

    // Two ways of serialization:
    // number 1:
    std::stringstream data;
    google::protobuf::util::SerializeDelimitedToOstream(a, &data);
    
    std::cout << "After serialization with delimited: {" << data.str() << "}\n" << std::endl;

    // Parse from data
    tutorial::Person parsed_person;
    parsed_person.ParseFromString(data.str());

    std::cout << "\nParsed person:\n" << parsed_person.DebugString() << std::endl;

    client.send_message(data.str());

    // send again
    client.send_message(data.str());

    return 0;
}