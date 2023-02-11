#include "serial.h"

#include <cerrno>
#include <cstdarg>
#include <cstring>

#include <iomanip>
#include <iostream>

#include <fcntl.h>

Serial::Serial(const char *file, speed_t baud_rate) {
    fd = open(file, O_RDWR | O_NOCTTY);

    if (fd < 0) {
        std::cerr << "Could not open tty \"" << file << "\": " << std::strerror(errno) << std::endl;
        return;
    }

    struct termios tty{};

    if(tcgetattr(fd, &tty) != 0)
    {
        std::cerr << "Could not read tty attributes: " << std::strerror(errno) << std::endl;
        return;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ECHOE;
    tty.c_lflag &= ~ECHONL;
    tty.c_lflag &= ~ISIG;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
    tty.c_oflag &= ~OPOST;
    tty.c_oflag &= ~ONLCR;
    //tty.c_cc[VTIME] = 10;
    //tty.c_cc[VMIN] = 1;

    cfsetispeed(&tty, baud_rate);
    cfsetospeed(&tty, baud_rate);

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        std::cerr << "Could not raw_write tty attributes: " << std::strerror(errno) << std::endl;
        return;
    }
}

ssize_t Serial::raw_write(const void *data, size_t size) {
    size_t i = 0;
    ssize_t bytes_written;
    auto bytes = (uint8_t *) data;

    do {
        bytes_written = ::write(fd, bytes + i, size - i);

        if (bytes_written < 0)
            return -errno;

        i += bytes_written;
    } while(i < size);

    return static_cast<ssize_t>(size);
}

//
// 'Command' : [ [ arg_list ],                [type_list],                    [enhanced precision]   ]
// 'SPED'    : [ ['f_vel'],                   [float],                        [False]                ] - Speed command -
// 'STER'    : [ ['f_angle'],                 [float],                        [False]                ] - Steer command -
// 'BRAK'    : [ ['f_angle' ],                [float],                        [False]                ] - Brake command -
// 'PIDA'    : [ ['activate' ],               [bool],                         [False]                ] - Activate PID control -
// 'ENPB'    : [ ['activate' ],               [bool],                         [False]                ] - Activate encoder publisher -
// 'PIDS'    : [ ['kp', 'ki', 'kd', 'tf' ],   [float, float, float, float],   [True]                 ] - Pass PID values -
// 'MOVE'    : [ ['distance', 'speed'],       [float, float],                 [True]                 ] - Set distance to travel and the speed -

constexpr size_t COMMAND_MAX_ARGS = 4;
struct {
    int nargs;
    char args[COMMAND_MAX_ARGS];
} command_args[] = {
        {0, {}},
        {1, {'f'}},
        {1, {'f'}},
        {1, {'f'}},
        {1, {'b'}},
        {1, {'b'}},
        {4, {'d', 'd', 'd', 'd'}},
        {2, {'d', 'd'}}
};

ssize_t Serial::write_command(Command command, ...)
{
    std::va_list args;

    int command_number = (int) command;

    va_start(args, command_args[command_number].nargs);

    std::ostringstream output;

    output << '#' << command_number << ':';

    for (int i = 0; i < command_args[command_number].nargs; i++) {
        switch (command_args[command_number].args[i]) {
            case 'f':
                output << std::fixed << std::setprecision(2) << va_arg(args, double) << ";";
                break;
            case 'd':
                output << std::fixed << std::setprecision(5) << va_arg(args, double) << ";";
                break;
            case 'b':
                output << va_arg(args, int) << ";";
                break;
        }
    }

    output << ";\r\n";

    std::string str = output.str();

    return raw_write(str.c_str(), str.length());
}

int Serial::read_char() const
{
    char b;
    ssize_t ret;

    ret = read(fd, &b, 1);

    if (ret < 0) {
        std::cerr << "Serial read error: " << std::strerror(errno) << std::endl;
        return -1;
    }

    if (ret == 0)
        return -1;

    //std::cout << "read " << b << " (" << (int) (std::uint8_t) b << ")" << std::endl;

    return b;
}

void Serial::consume_char(char expected) {
    int c;

    if ((c = read_char()) != expected) {
        if (c == -1) {
            std::cerr << "Serial closed unexpectedly" << std::endl;
        } else {
            if (expected != '\0')
                std::cerr << "Unexpected '" << (char) c << "' in serial stream (expected '" << expected << "')" << std::endl;
        }
    }
}

std::string Serial::read_response()
{
    int c, i;
    char msg[256];

    consume_char('@');
    //consume_char('\0');
    //consume_char(':');

    for (i = 0; i < 255 && ((c = read_char()) != '\r') && c != -1; i++) {
        msg[i] = (char) c;
    }

    msg[i] = '\0';

    consume_char('\n');

    return {msg};
}

std::string Serial::read_line()
{
    int c, i;
    char msg[256];

    for (i = 0; i < 255 && ((c = read_char()) != '\r') && c != -1; i++) {
        msg[i] = (char) c;
    }

    msg[i] = '\0';

    consume_char('\n');

    return {msg};
}
