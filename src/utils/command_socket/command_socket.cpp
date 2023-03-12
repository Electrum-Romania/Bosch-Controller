#include <utils/command_socket/command_socket.h>

CommandSocket::CommandSocket(const std::string& host, uint16_t port)
    : Loggable("CS", 'C'), conn({host, port}), thread([this] () {
        run();
    })
{
}

void CommandSocket::register_command(const std::string& cname, void *object, CommandCallback f)
{
    log(LogLevel::INFO, "registering command " + cname);
    commands.insert({cname, {object, f}});
}

void CommandSocket::run()
{
    // check connection initialization
    if (!conn) {
        log(LogLevel::ERROR, "Error connecting to server: " + conn.last_error_str());

        dead = true;
    }

    while (!dead) {
        std::string line = conn.read_line();

        size_t delimiter_positon = line.find('=');

        std::string cname = line.substr(0, delimiter_positon);
        std::string arg = line.substr(delimiter_positon + 1, line.length() - 2 - delimiter_positon);

        const Command &c = commands.at(cname);

        c.callback(c.object, cname, arg);
    }
}
