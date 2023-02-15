//
// Created by grffn on 2/15/23.
//

#include "motors.h"

Motors::Motors(Serial &serial)
    : nucleo(serial), current_motor_speed(0.0)
{

}

void Motors::compute_frame()
{
    double delta_forward = 0.01 * (pdata->pressed_keys.w - pdata->pressed_keys.s);

    double old_speed = current_motor_speed;

    if (-delta_forward > current_motor_speed)
        current_motor_speed = 0.00;
    else if (current_motor_speed + delta_forward > 0.5)
        current_motor_speed = 0.5;
    else if (delta_forward != 0)
        current_motor_speed += delta_forward;

    //std::cout << current_motor_speed << std::endl;
    if (current_motor_speed != old_speed) {
        nucleo.write_command(Serial::Command::SPED, current_motor_speed);
        std::string response = nucleo.read_response();

        if (response != "ack")
            std::cerr << "Nucleo error : " << response;
    }
}
