#include "command_socket.h"
#include "../../controller.h"

command_socket::command_socket(const std::string& host, uint16_t port)
    : conn({host, port}), dead(false), thread([this] () {
        run();
    })
{
    screen_index = logger.request_screen('c', "CS");
}

void command_socket::register_command(const std::string& cname, void *object, command_callback f)
{
    logger.log(screen_index, LogLevel::INFO, "registering command " + cname);
    commands.insert({cname, {object, f}});
}

void command_socket::run()
{
    // check connection initialization
    if (!conn) {
        logger.log(screen_index, LogLevel::ERROR, "Error connecting to server: " + conn.last_error_str());

        dead = true;
    }

    while (!dead) {
        std::string line = conn.read_line();

        size_t delimiter_positon = line.find('=');

        std::string cname = line.substr(0, delimiter_positon);
        std::string arg = line.substr(delimiter_positon + 1, line.length() - 2);

        const command &c = commands.at(cname);

        c.callback(c.object, cname, arg);
    }
}
