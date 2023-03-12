#include "motors.h"

Motors::Motors(Serial &serial)
        : IOtask("motors", 'm'), nucleo(serial), current_motor_speed(0.0)
{
    current_motor_speed_watch_value_index = logger.register_watch_value(watch_index, "Motor speed", 6);
    current_steer_watch_value_index = logger.register_watch_value(watch_index, "Steer value", 6);
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

    if (current_motor_speed != old_speed) {
        logger.set_watch_value(watch_index, current_motor_speed_watch_value_index,
                               nclogger::to_string(current_motor_speed, 6, [](auto &ss) {
                                   fixed(ss);
                                   ss.precision(2);
                               }));

        nucleo.write_command(Serial::Command::SPED, current_motor_speed);
        std::string response = nucleo.read_response();

        if (response != "1:ack;;")
            logger.log(screen_index, LogLevel::WARN, "Nucleo error: " + response);
    }

    double old_ster = current_ster;

    if (options->keyboard_steer_control) {
        double delta_right = 0.01 * (pdata->pressed_keys.d - pdata->pressed_keys.a);

        if (current_ster + delta_right < -0.5)
            current_ster = -0.5;
        else if (current_ster + delta_right > 0.5)
            current_ster = 0.5;
        else if (delta_right != 0)
            current_ster += delta_right;

        if (current_ster != old_ster) {
            logger.set_watch_value(watch_index, current_steer_watch_value_index,
                                   nclogger::to_string(current_ster, 6, [](auto &ss) {
                                       fixed(ss);
                                       ss.precision(2);
                                   }));

            nucleo.write_command(Serial::Command::STER, current_ster);
            std::string response = nucleo.read_response();

            if (response != "2:ack;;")
                logger.log(screen_index, LogLevel::WARN, "Nucleo error: " + response);
        }
    } else {
        current_ster = old_ster;
    }

    if (current_ster != old_ster) {
        logger.set_watch_value(watch_index, current_steer_watch_value_index,
                               nclogger::to_string(current_ster, 6, [](auto &ss) {
                                   fixed(ss);
                                   ss.precision(2);
                               }));

        nucleo.write_command(Serial::Command::STER, current_ster);
        std::string response = nucleo.read_response();

        if (response != "2:ack;;")
            logger.log(screen_index, LogLevel::WARN, "Nucleo error: " + response);
    }
}
