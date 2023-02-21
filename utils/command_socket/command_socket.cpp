#include "command_socket.h"

command_socket::command_socket(const std::string& host, uint16_t port)
    : conn({host, port}), dead(false), thread([this] () {
        run();
    })
{}

void command_socket::register_command(const std::string& cname, void *object, command_callback f)
{
    std::cout << "registering command " << cname << std::endl;
    commands.insert({cname, {object, f}});
}

void command_socket::run()
{
    // check connection initialization
    if (!conn) {
        std::cerr << "Error connecting to server: "
                  << conn.last_error_str() << std::endl;

        dead = true;
    }

    while (!dead) {
        std::string line = conn.read_line();

        size_t delimiter_positon = line.find('=');

        std::string cname = line.substr(0, delimiter_positon);
        std::string arg = line.substr(delimiter_positon + 1, line.length() - 2 - delimiter_positon);

        std::cout << "(" << cname << ")=(" << arg << ")" << std::endl;

        const command &c = commands.at(cname);

        c.callback(c.object, cname, arg);
    }
}

