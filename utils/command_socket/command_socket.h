#ifndef UTILS_COMMAND_SOCKET_H
#define UTILS_COMMAND_SOCKET_H

#include <unordered_map>
#include <thread>

#include "line_reader.h"

typedef void (*command_callback) (void *, std::string);

class command_socket {
private:
    struct command {
        void *object;
        command_callback callback;
    };

    line_reader conn;

    bool dead;

    std::thread thread;

    std::unordered_map<std::string, command> commands;

public:
    command_socket(const std::string& host, uint16_t port);

    void run();

    void register_command(const std::string &cname, void *object, command_callback f);
};

#endif //UTILS_COMMAND_SOCKET_H
