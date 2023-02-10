#ifndef UTILS_SERIAL_H
#define UTILS_SERIAL_H

#include <termios.h>
#include <unistd.h>

class Serial {
private:
    int fd;

public:
    enum class Command {
        SPED = 1,
        STER = 2,
        BRAK = 3,
        PIDA = 4,
        ENPB = 5,
        PIDS = 6,
        MOVE = 7,
    };

    explicit Serial(const char *file = "/dev/ttyACM0", speed_t baud_rate = B19200);

    ssize_t raw_write(const void *data, size_t size);

    //ssize_t write_command(int command_number, float arg);

    //ssize_t write_command(int command_number, bool arg);

    ssize_t write_command(Command command_number, ...);
};

//
// 'Command' : [ [ arg_list ],                [type_list],                    [enhanced precision]   ]
// 'SPED'    : [ ['f_vel'],                   [float],                        [False]                ] - Speed command -
// 'STER'    : [ ['f_angle'],                 [float],                        [False]                ] - Steer command -
// 'BRAK'    : [ ['f_angle' ],                [float],                        [False]                ] - Brake command -
// 'PIDA'    : [ ['activate' ],               [bool],                         [False]                ] - Activate PID control -
// 'ENPB'    : [ ['activate' ],               [bool],                         [False]                ] - Activate encoder publisher -
// 'PIDS'    : [ ['kp', 'ki', 'kd', 'tf' ],   [float, float, float, float],   [True]                 ] - Pass PID values -
// 'MOVE'    : [ ['distance', 'speed'],       [float, float],                 [True]                 ] - Set distance to travel and the speed -


#endif // UTILS_SERIAL_H
