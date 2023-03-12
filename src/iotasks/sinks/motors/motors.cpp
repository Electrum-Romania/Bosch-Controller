#include <iotasks/sinks/motors//motors.h>

enum {
    watch_current_motor_speed,
    watch_current_steer,
};

static
std::vector<Loggable::WatchPair> watch_pairs {
    {watch_current_motor_speed, "Motor speed", 6},
    {watch_current_steer, "Steer", 6},
};

Motors::Motors(Serial &serial)
    : Sink("motors", 'm', watch_pairs), nucleo(serial)
{}

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
        set_watch_value(watch_current_motor_speed, current_motor_speed, 2);

        nucleo.write_command(Serial::Command::SPED, current_motor_speed);
        std::string response = nucleo.read_response();

        if (response != "1:ack;;")
            log(LogLevel::WARN, "Nucleo error: " + response);
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
            set_watch_value(watch_current_steer, current_ster, 2);

            nucleo.write_command(Serial::Command::STER, current_ster);
            std::string response = nucleo.read_response();

            if (response != "2:ack;;")
                log(LogLevel::WARN, "Nucleo error: " + response);
        }
    } else {
        current_ster = old_ster;
    }
}
