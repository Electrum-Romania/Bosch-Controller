#include <sockpp/socket.h>

#include "utils/serial/serial.h"
#include "utils/command_socket/command_socket.h"

#include <cmath>

#include <iostream>
#include <nclogger/logger.hpp>

[[noreturn]] void sched();

void test_serial()
{
    Serial serial;

    for (double i = 0.0; i < 5.0; i += 0.1) {
        serial.write_command(Serial::Command::SPED, i);

        std::cout << i << " " << serial.read_response() << std::endl;
    }
}

void cb(void *, const std::string&, const std::string& arg)
{
    std::cout << "(" << arg << ")" << std::endl;
}

nclogger::Logger logger;

int main()
{
    sockpp::initialize();


	sched();
}
