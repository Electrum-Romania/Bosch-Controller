#include <sockpp/socket.h>

#include "utils/serial/serial.h"

#include <cmath>

#include <iostream>

[[noreturn]] void sched();

void test_serial()
{
    Serial serial;

    for (double i = 0.0; i < 5.0; i += 0.1) {
        serial.write_command(Serial::Command::SPED, i);

        std::cout << i << " " << serial.read_response() << std::endl;
    }
}

int main()
{
    sockpp::initialize();

    test_serial();

	//sched();
}
