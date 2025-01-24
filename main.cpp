#include "connections/TCPClient.hpp"
#include "protos/addressbook.pb.h"
#include <filesystem>
#include <fstream> 
#include <source_location>
#include <string>
#include "google/protobuf/util/delimited_message_util.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace fs = std::filesystem;
using namespace std::literals;
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
    spdlog::set_level(spdlog::level::info);
    const std::string LOGGER_NAME = "console"s;
    auto logger = spdlog::stdout_color_mt(LOGGER_NAME);

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

    spdlog::get(LOGGER_NAME)->debug("Before Serialization\n{}", a.DebugString());

    // Get path to the the current source location
    fs::path current_path = std::source_location::current().file_name();
    fs::path output_file_path = current_path.parent_path() / "test.txt";

    spdlog::get(LOGGER_NAME)->info("Writing to file: {}", output_file_path.c_str());

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
    spdlog::get(LOGGER_NAME)->debug("After serialization with delimited: {}", data.str());

    // Parse from data
    tutorial::Person parsed_person;
    parsed_person.ParseFromString(data.str());

    spdlog::get(LOGGER_NAME)->debug("Parsed person from own serialization:\n{}", parsed_person.DebugString());

    client.send_message(data.str());

    // send again
    client.send_message(data.str());

    return 0;
}