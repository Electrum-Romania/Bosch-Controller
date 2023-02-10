#include <sockpp/socket.h>

#include "utils/serial/serial.h"

[[noreturn]] void sched();

void test_serial()
{
    Serial serial("/dev/stdin", B38400);

    serial.write_command(Serial::Command::SPED, 0.1);
}

int main()
{
    sockpp::initialize();

    test_serial();

	sched();
}
