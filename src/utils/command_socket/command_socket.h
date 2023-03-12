#ifndef UTILS_COMMAND_SOCKET_H
#define UTILS_COMMAND_SOCKET_H

#include <controller.h>

#include <unordered_map>
#include <thread>

#include <utils/command_socket/line_reader.h>
#include <utils/loggable/loggable.h>

typedef void (*CommandCallback) (void *, const std::string&, const std::string&);

class CommandSocket : public Loggable {
private:
    struct Command {
        void *object;
        CommandCallback callback;
    };

    bool dead = false;
    LineReader conn;
    std::thread thread;

    std::unordered_map<std::string, Command> commands;

public:
    CommandSocket(const std::string& host, uint16_t port);

    void run();
    void register_command(const std::string &cname, void *object, CommandCallback f);
};

#endif //UTILS_COMMAND_SOCKET_H
